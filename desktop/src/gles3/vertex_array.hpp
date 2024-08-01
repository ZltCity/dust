#pragma once

#include <span>

#include <GLES3/gl32.h>

#include "bind_guard.hpp"
#include "buffer.hpp"

namespace dust::gles3
{

struct VertexAttrib
{
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	GLsizeiptr offset;
};

class VertexArray
{
public:
	VertexArray();
	VertexArray(const Buffer &vbo, std::span<const VertexAttrib> attributes_);
	VertexArray(const VertexArray &) = delete;
	VertexArray(VertexArray &&other) noexcept;
	~VertexArray() noexcept;

	VertexArray &operator=(const VertexArray &) = delete;
	VertexArray &operator=(VertexArray &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLuint handle() const;

	void attributes(const Buffer &vbo, std::span<const VertexAttrib> attributes_);

	void swap(VertexArray &other) noexcept;
	void reset() noexcept;

private:
	[[nodiscard]] static GLuint create();

	GLuint m_handle;
};

template<>
struct Binding<VertexArray>
{
	explicit Binding(const VertexArray &array);

	void bind() const;
	void bind(GLuint handle_) const;

	[[nodiscard]] GLuint current() const;

	GLuint handle;
};

} // namespace dust::gles3