#include <utility>

#include <dust/gles3/vertex_array.hpp>

namespace dust::gles3
{

VertexArray::VertexArray() : m_handle {}
{}

VertexArray::VertexArray(std::span<const VertexAttrib> attributes_) : m_handle(create())
{
	attributes(attributes_);
}

VertexArray::VertexArray(VertexArray &&other) noexcept : m_handle {}
{
	swap(other);
}

VertexArray::~VertexArray() noexcept
{
	reset();
}

VertexArray &VertexArray::operator=(VertexArray &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

VertexArray::operator bool() const
{
	return m_handle > 0;
}

GLuint VertexArray::handle() const
{
	return m_handle;
}

void VertexArray::attributes(std::span<const VertexAttrib> attributes_)
{
	auto bind = BindGuard(*this);
	auto index = GLuint {};

	for (const auto &attr : attributes_)
	{
		glVertexAttribPointer(
			index, attr.size, attr.type, attr.normalized, attr.stride, reinterpret_cast<void *>(attr.offset));
		glEnableVertexAttribArray(index);

		++index;
	}
}

void VertexArray::swap(VertexArray &other) noexcept
{
	std::swap(m_handle, other.m_handle);
}

void VertexArray::reset() noexcept
{
	if (m_handle)
	{
		glDeleteVertexArrays(1, &m_handle);

		m_handle = {};
	}
}

GLuint VertexArray::create()
{
	auto handle = GLuint {};

	glGenVertexArrays(1, &handle);

	return handle;
}

Binding<VertexArray>::Binding(const VertexArray &array) : handle(array.handle())
{}

void Binding<VertexArray>::bind() const
{
	glBindVertexArray(handle);
}

void Binding<VertexArray>::bind(GLuint handle_) const
{
	glBindVertexArray(handle_);
}

GLuint Binding<VertexArray>::current() const
{
	auto binding = GLint {};

	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &binding);

	return static_cast<GLuint>(binding);
}

} // namespace dust::gles3