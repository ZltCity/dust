#pragma once

#include <GLES3/gl32.h>

#include <dust/gles3/bind_guard.hpp>

namespace dust::gles3
{

class Buffer
{
public:
	Buffer();
	explicit Buffer(GLenum target);
	template<class TContainer>
	Buffer(GLenum target, const TContainer &data_, GLenum usage);
	Buffer(const Buffer &) = delete;
	Buffer(Buffer &&other) noexcept;
	~Buffer() noexcept;

	Buffer &operator=(const Buffer &) = delete;
	Buffer &operator=(Buffer &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLenum target() const;
	[[nodiscard]] GLuint handle() const;

	template<class TContainer>
	void data(GLenum target, const TContainer &data_, GLenum usage);
	template<class TContainer>
	void subData(size_t offset, const TContainer &data_);

	void swap(Buffer &other) noexcept;
	void reset() noexcept;

private:
	[[nodiscard]] static GLuint create();

	GLenum m_target;
	GLuint m_handle;
};

template<>
struct Binding<Buffer>
{
	explicit Binding(const Buffer &buffer);

	void bind() const;
	void bind(GLuint handle_) const;

	[[nodiscard]] GLuint current() const;

	GLenum target;
	GLuint handle;
};

template<class TContainer>
Buffer::Buffer(GLenum target, const TContainer &data_, GLenum usage) : m_target(target), m_handle(create())
{
	data(m_target, data_, usage);
}

template<class TContainer>
void Buffer::data(GLenum target, const TContainer &data_, GLenum usage)
{
	auto bind = BindGuard(*this);

	glBufferData(
		m_target, static_cast<GLsizeiptr>(sizeof(typename TContainer::value_type) * data_.size()), data_.data(), usage);
}

template<class TContainer>
void Buffer::subData(size_t offset, const TContainer &data_)
{
	auto bind = BindGuard(*this);

	glBufferSubData(
		m_target, static_cast<GLintptr>(offset),
		static_cast<GLsizeiptr>(sizeof(typename TContainer::value_type) * data_.size()), data_.data());
}

} // namespace dust::gles3