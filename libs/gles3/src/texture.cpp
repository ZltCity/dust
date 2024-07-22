#include <stdexcept>
#include <utility>

#include <dust/gles3/texture.hpp>

namespace dust::gles3
{

Texture::Texture() : m_target {}, m_handle {}
{}

Texture::Texture(GLenum target) : m_target(target), m_handle(create())
{}

Texture::Texture(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
	: m_target(target), m_handle(create())
{
	storage2D(levels, internalFormat, width, height);
}

Texture::Texture(Texture &&other) noexcept : m_target {}, m_handle {}
{
	swap(other);
}

Texture::~Texture() noexcept
{
	reset();
}

Texture &Texture::operator=(Texture &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Texture::operator bool() const
{
	return m_handle > 0;
}

GLenum Texture::target() const
{
	return m_target;
}

GLuint Texture::handle() const
{
	return m_handle;
}

void Texture::storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
{
	auto bind = BindGuard(*this);

	glTexStorage2D(m_target, levels, internalFormat, width, height);
}

void Texture::activeTexture(GLint unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
}

void Texture::swap(Texture &other) noexcept
{
	std::swap(m_target, other.m_target);
	std::swap(m_handle, other.m_handle);
}

void Texture::reset() noexcept
{
	if (m_handle)
	{
		glDeleteTextures(1, &m_handle);

		m_target = {};
		m_handle = {};
	}
}

GLuint Texture::create()
{
	auto handle = GLuint {};

	glGenTextures(1, &handle);

	return handle;
}

Binding<Texture>::Binding(const Texture &texture) : target(texture.target()), handle(texture.handle())
{}

void Binding<Texture>::bind() const
{
	glBindTexture(target, handle);
}

void Binding<Texture>::bind(GLuint handle_) const
{
	glBindTexture(target, handle_);
}

GLuint Binding<Texture>::current() const
{
	auto name = GLenum {};
	auto binding = GLint {};

	// clang-format off
	switch (target)
	{
		case GL_TEXTURE_2D: 					{	name = GL_TEXTURE_BINDING_2D; 						break;	}
		case GL_TEXTURE_2D_MULTISAMPLE: 		{	name = GL_TEXTURE_BINDING_2D_MULTISAMPLE; 			break;	}
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:	{	name = GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;		break;	}
		case GL_TEXTURE_3D: 					{	name = GL_TEXTURE_BINDING_3D; 						break;	}
		case GL_TEXTURE_2D_ARRAY: 				{	name = GL_TEXTURE_BINDING_2D_ARRAY; 				break;	}
		case GL_TEXTURE_CUBE_MAP: 				{	name = GL_TEXTURE_BINDING_CUBE_MAP; 				break;	}
		case GL_TEXTURE_CUBE_MAP_ARRAY: 		{	name = GL_TEXTURE_BINDING_CUBE_MAP_ARRAY; 			break;	}
		case GL_TEXTURE_BUFFER: 				{	name = GL_TEXTURE_BINDING_BUFFER; 					break;	}

		default: throw std::logic_error("Undefined texture target.");
	}
	// clang-format on

	glGetIntegerv(name, &binding);

	return static_cast<GLuint>(binding);
}

} // namespace dust::gles3
