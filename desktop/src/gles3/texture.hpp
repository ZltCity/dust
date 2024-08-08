#pragma once

#include <GLES3/gl32.h>

#include "bind_guard.hpp"

namespace dust::gles3
{

class Texture
{
public:
	Texture();
	explicit Texture(GLenum target);
	Texture(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
	Texture(const Texture &) = delete;
	Texture(Texture &&other) noexcept;
	~Texture() noexcept;

	Texture &operator=(const Texture &) = delete;
	Texture &operator=(Texture &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLenum target() const;
	[[nodiscard]] GLuint handle() const;

	void storage2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
	template<class TContainer>
	void subImage2D(
		GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
		const TContainer &data);

	void texParameter(GLenum name, GLint value);

	void swap(Texture &other) noexcept;
	void reset() noexcept;

private:
	[[nodiscard]] static GLuint create();

	GLenum m_target;
	GLuint m_handle;
};

template<>
struct Binding<Texture>
{
	explicit Binding(const Texture &texture);
	Binding(const Texture &texture, GLenum unit);

	void bind() const;
	void bind(GLuint handle_) const;

	[[nodiscard]] GLuint current() const;

	GLenum target;
	GLuint handle;
	GLenum unit = GL_TEXTURE0;
};

template<class TContainer>
void Texture::subImage2D(
	GLint level, GLint xOffset, GLint yOffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
	const TContainer &data)
{
	auto bind = BindGuard(*this);

	glTexSubImage2D(m_target, level, xOffset, yOffset, width, height, format, type, data.data());
}

} // namespace dust::gles3