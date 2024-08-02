#pragma once

#include <span>

#include <GLES3/gl32.h>

#include "bind_guard.hpp"

namespace dust::gles3
{

class Buffer
{
public:
	Buffer();
	explicit Buffer(GLenum target);
	template<class TCollection>
	Buffer(GLenum target, GLenum usage, const TCollection &data_);
	Buffer(const Buffer &) = delete;
	Buffer(Buffer &&other) noexcept;
	~Buffer() noexcept;

	Buffer &operator=(const Buffer &) = delete;
	Buffer &operator=(Buffer &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLenum target() const;
	[[nodiscard]] GLuint handle() const;

	template<class TCollection>
	void data(GLenum usage, const TCollection &data_);
	template<class TCollection>
	void subData(size_t offset, const TCollection &data_);

	template<class... TArgs>
	void push(GLenum value, TArgs &&...args);
	template<class... TArgs>
	void update(size_t offset, TArgs &&...args);

	void swap(Buffer &other) noexcept;
	void reset() noexcept;

private:
	[[nodiscard]] static GLuint create();
	template<class... TArgs>
	void writeArgs(size_t offset, TArgs &&...args);

	GLenum m_target;
	GLuint m_handle;
};

template<>
struct Binding<Buffer>
{
	explicit Binding(const Buffer &buffer);
	Binding(const Buffer &buffer, GLuint bindingPoint_);

	void bind() const;
	void bind(GLuint handle_) const;
	void bindBase() const;
	void bindBase(GLuint handle_) const;

	[[nodiscard]] GLuint current() const;

	GLenum target;
	GLuint handle;
	GLuint bindingPoint = {};
};

template<class TCollection>
Buffer::Buffer(GLenum target, GLenum usage, const TCollection &data_) : m_target(target), m_handle(create())
{
	data(usage, data_);
}

template<class TCollection>
void Buffer::data(GLenum usage, const TCollection &data_)
{
	auto bind = BindGuard(*this);

	glBufferData(
		m_target, static_cast<GLsizeiptr>(sizeof(typename TCollection::value_type) * data_.size()), data_.data(),
		usage);
}

template<class TCollection>
void Buffer::subData(size_t offset, const TCollection &data_)
{
	auto bind = BindGuard(*this);

	glBufferSubData(
		m_target, static_cast<GLintptr>(offset),
		static_cast<GLsizeiptr>(sizeof(typename TCollection::value_type) * data_.size()), data_.data());
}

template<class... TArgs>
void Buffer::push(GLenum usage, TArgs &&...args)
{
	const auto dataLength = (0 + ... + sizeof(args));
	auto bind = BindGuard(*this);

	glBufferData(m_target, static_cast<GLsizeiptr>(dataLength), nullptr, usage);
	writeArgs(0, std::forward<TArgs>(args)...);
}

template<class... TArgs>
void Buffer::update(size_t offset, TArgs &&...args)
{
	auto bind = BindGuard(*this);

	writeArgs(offset, std::forward<TArgs>(args)...);
}

template<class... TArgs>
void Buffer::writeArgs(size_t offset, TArgs &&...args)
{
	const auto writeValue = [this, &offset](auto &&value) {
		glBufferSubData(m_target, static_cast<GLintptr>(offset), sizeof(value), &value);
		offset += sizeof(value);
	};

	(writeValue(std::forward<TArgs>(args)), ...);
}

} // namespace dust::gles3