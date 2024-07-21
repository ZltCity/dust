#pragma once

#include <cstddef>

#include <span>
#include <string>

#include <GLES3/gl32.h>

namespace dust::gles3
{

class Shader
{
public:
	Shader();
	explicit Shader(GLenum type);
	Shader(GLenum type, std::span<const std::byte> source_);
	Shader(const Shader &) = delete;
	Shader(Shader &&other) noexcept;
	~Shader() noexcept;

	Shader &operator=(const Shader &) = delete;
	Shader &operator=(Shader &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLenum type() const;
	[[nodiscard]] GLenum handle() const;

	[[nodiscard]] std::string log() const;

	void source(std::span<const std::byte> source_);
	[[nodiscard]] bool compile();

	void swap(Shader &other) noexcept;
	void reset() noexcept;

private:
	GLenum m_type;
	GLuint m_handle;
};

} // namespace dust::gles3