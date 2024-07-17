#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <thread>

namespace dust::threading
{

class QueueStopped final : public std::runtime_error
{
public:
	QueueStopped();
};

template<class T>
class BasicQueue
{
public:
	using Container = std::deque<T>;

	BasicQueue() : m_maxSize(std::numeric_limits<typename Container::size_type>::max())
	{}
	explicit BasicQueue(typename Container::size_type maxSize_) : m_maxSize(maxSize_)
	{
		if (maxSize_ == 0)
		{
			throw std::invalid_argument("Maximum queue size cannot be 0.");
		}
	}
	BasicQueue(const BasicQueue &) = delete;
	virtual ~BasicQueue() noexcept = 0;

	BasicQueue &operator=(const BasicQueue &) = delete;

	[[nodiscard]] auto maxSize() const
	{
		return m_maxSize;
	}

protected:
	virtual void waitCanPush(std::unique_lock<std::mutex> &lock) = 0;
	virtual void waitCanPop(std::unique_lock<std::mutex> &lock) = 0;

	virtual void wakeupProducers() = 0;
	virtual void wakeupConsumers() = 0;

	const typename Container::size_type m_maxSize;

	std::mutex m_lock;
	Container m_items;
	std::atomic_flag m_stopFlag;
};

template<class T>
BasicQueue<T>::~BasicQueue() noexcept = default;

template<class T>
class InputQueue : virtual public BasicQueue<T>
{
public:
	virtual ~InputQueue() noexcept = 0;

	[[nodiscard]] T pop()
	{
		auto lock = std::unique_lock(this->m_lock);

		this->waitCanPop(lock);

		if (this->m_stopFlag.test() and this->m_items.empty())
		{
			throw QueueStopped();
		}

		return unsafePop();
	}

	[[nodiscard]] std::optional<T> tryPop()
	{
		auto lock = std::lock_guard(this->m_lock);

		if (this->m_items.empty())
		{
			return {};
		}

		return unsafePop();
	}

private:
	void waitCanPush(std::unique_lock<std::mutex> &lock) final
	{
		m_canPush.wait(lock, [this]() { return this->m_items.size() < this->m_maxSize or this->m_stopFlag.test(); });
	}

	void wakeupProducers() final
	{
		m_canPush.notify_all();
	}

	[[nodiscard]] T unsafePop()
	{
		auto item = std::move(this->m_items.front());

		this->m_items.pop_front();
		wakeupProducers();

		return item;
	}

	std::condition_variable m_canPush;
};

template<class T>
InputQueue<T>::~InputQueue() noexcept = default;

template<class T>
class OutputQueue : virtual public BasicQueue<T>
{
public:
	virtual ~OutputQueue() noexcept = 0;

	void push(T value)
	{
		auto lock = std::unique_lock(this->m_lock);

		this->waitCanPush(lock);

		if (this->m_stopFlag.test())
		{
			throw QueueStopped();
		}

		this->m_items.push_back(std::move(value));
		wakeupConsumers();
	}

	void forcePush(T value)
	{
		auto lock = std::lock_guard(this->m_lock);

		if (this->m_stopFlag.test())
		{
			throw QueueStopped();
		}

		this->m_items.push_back(std::move(value));

		if (this->m_items.size() > this->m_maxSize)
		{
			this->m_items.pop_front();
		}

		wakeupConsumers();
	}

	void stop()
	{
		auto lock = std::lock_guard(this->m_lock);

		this->m_stopFlag.test_and_set();
		this->wakeupProducers();
		wakeupConsumers();
	}

private:
	void waitCanPop(std::unique_lock<std::mutex> &lock) final
	{
		m_canPop.wait(lock, [this]() { return not this->m_items.empty() or this->m_stopFlag.test(); });
	}

	void wakeupConsumers() final
	{
		m_canPop.notify_all();
	}

	std::condition_variable m_canPop;
};

template<class T>
OutputQueue<T>::~OutputQueue() noexcept = default;

template<class T>
class Queue final : public InputQueue<T>, public OutputQueue<T>
{
public:
	using Container = typename BasicQueue<T>::Container;

	Queue() = default;
	explicit Queue(typename Container::size_type maxSize) : BasicQueue<T>(maxSize)
	{}
};

} // namespace dust::threading
