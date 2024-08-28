#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <memory>

#include "../threading/queue.hpp"

namespace dust::logging
{

enum class LogLevel
{
	Debug = 0,
	Info,
	Warning,
	Critical
};

class Log
{
public:
	using WriterCallback = std::function<void(const std::string &)>;

	Log(const Log &) = delete;
	~Log();

	Log &operator=(const Log &) = delete;

	void push(LogLevel level, const std::string &message);

	void severity(LogLevel level);
	void writer(const std::string &id, WriterCallback callback);

	static void debug(const std::string &message);
	static void info(const std::string &message);
	static void warning(const std::string &message);
	static void critical(const std::string &message);

	[[nodiscard]] static Log &instance();

private:
	using Clock = std::chrono::high_resolution_clock;

	struct Message
	{
		LogLevel level;
		Clock::time_point time;
		std::string message;
	};

	struct Context
	{
		std::mutex lock;
		std::map<std::string, WriterCallback> writers;
		LogLevel severity = LogLevel::Info;
		threading::Queue<Message> messages;
	};

	Log();

	static void writeRoutine(std::stop_token stop, std::shared_ptr<Context> context);

	std::shared_ptr<Context> m_context;
	std::jthread m_writeThread;
};

} // namespace dust::logging