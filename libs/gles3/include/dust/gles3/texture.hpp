#pragma once

#include <GLES3/gl32.h>

#include <dust/gles3/bind_guard.hpp>

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
	void image2D(
		GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type,
		const TContainer &data);

	static void activeTexture(GLint unit);

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

	void bind() const;
	void bind(GLuint handle_) const;

	[[nodiscard]] GLuint current() const;

	GLenum target;
	GLuint handle;
};

template<class TContainer>
void Texture::image2D(
	GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type,
	const TContainer &data)
{
	auto bind = BindGuard(*this);

	glTexImage2D(m_target, level, internalFormat, width, height, 0, format, type, data.data());
}

} // namespace dust::gles3