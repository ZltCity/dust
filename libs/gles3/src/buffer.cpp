#include <stdexcept>

#include <dust/gles3/buffer.hpp>

namespace dust::gles3
{

Buffer::Buffer() : m_target {}, m_handle {}
{}

Buffer::Buffer(GLenum target) : m_target(target), m_handle(create())
{}

Buffer::Buffer(Buffer &&other) noexcept : m_target {}, m_handle {}
{
	swap(other);
}

Buffer::~Buffer() noexcept
{
	reset();
}

Buffer &Buffer::operator=(Buffer &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Buffer::operator bool() const
{
	return m_handle > 0;
}

GLenum Buffer::target() const
{
	return m_target;
}

GLuint Buffer::handle() const
{
	return m_handle;
}

void Buffer::swap(Buffer &other) noexcept
{
	std::swap(m_target, other.m_target);
	std::swap(m_handle, other.m_handle);
}

void Buffer::reset() noexcept
{
	if (m_handle)
	{
		glDeleteBuffers(1, &m_handle);

		m_target = {};
		m_handle = {};
	}
}

GLuint Buffer::create()
{
	auto handle = GLuint {};

	glGenBuffers(1, &handle);

	return handle;
}

Binding<Buffer>::Binding(const Buffer &buffer) : target(buffer.target()), handle(buffer.handle())
{}

void Binding<Buffer>::bind() const
{
	glBindBuffer(target, handle);
}

void Binding<Buffer>::bind(GLuint handle_) const
{
	glBindBuffer(target, handle_);
}

GLuint Binding<Buffer>::current() const
{
	auto name = GLenum {};
	auto binding = GLint {};

	// clang-format off
	switch (target)
	{
		case GL_ARRAY_BUFFER: 				{	name = GL_ARRAY_BUFFER_BINDING; 				break;	}
		case GL_ATOMIC_COUNTER_BUFFER: 		{	name = GL_ATOMIC_COUNTER_BUFFER_BINDING; 		break;	}
		case GL_COPY_READ_BUFFER: 			{	name = GL_COPY_READ_BUFFER_BINDING; 			break;	}
		case GL_COPY_WRITE_BUFFER: 			{	name = GL_COPY_WRITE_BUFFER_BINDING; 			break;	}
		case GL_DISPATCH_INDIRECT_BUFFER: 	{	name = GL_DISPATCH_INDIRECT_BUFFER_BINDING; 	break;	}
		case GL_DRAW_INDIRECT_BUFFER: 		{	name = GL_DRAW_INDIRECT_BUFFER_BINDING; 		break;	}
		case GL_ELEMENT_ARRAY_BUFFER: 		{	name = GL_ELEMENT_ARRAY_BUFFER_BINDING; 		break;	}
		case GL_PIXEL_PACK_BUFFER: 			{	name = GL_PIXEL_PACK_BUFFER_BINDING; 			break;	}
		case GL_PIXEL_UNPACK_BUFFER: 		{	name = GL_PIXEL_UNPACK_BUFFER_BINDING;			break;	}
		case GL_SHADER_STORAGE_BUFFER: 		{	name = GL_SHADER_STORAGE_BUFFER_BINDING; 		break;	}
		case GL_TEXTURE_BUFFER: 			{	name = GL_TEXTURE_BUFFER_BINDING; 				break;	}
		case GL_TRANSFORM_FEEDBACK_BUFFER: 	{	name = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING; 	break;	}
		case GL_UNIFORM_BUFFER: 			{	name = GL_UNIFORM_BUFFER_BINDING; 				break;	}

		default: throw std::logic_error("Undefined buffer target.");
	}
	// clang-format on

	glGetIntegerv(name, &binding);

	return static_cast<GLuint>(binding);
}

} // namespace dust::gles3