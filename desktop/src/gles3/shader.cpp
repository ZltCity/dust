#include "shader.hpp"

namespace dust::gles3
{

Shader::Shader() : m_type {}, m_handle {}
{}

Shader::Shader(GLenum type) : m_type(type), m_handle(glCreateShader(m_type))
{}

Shader::Shader(GLenum type, std::span<const std::byte> source_) : m_type(type), m_handle(glCreateShader(m_type))
{
	source(source_);
}

Shader::Shader(Shader &&other) noexcept : m_type {}, m_handle {}
{
	swap(other);
}

Shader::~Shader() noexcept
{
	reset();
}

Shader &Shader::operator=(Shader &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Shader::operator bool() const
{
	return m_handle > 0;
}

GLenum Shader::type() const
{
	return m_type;
}

GLenum Shader::handle() const
{
	return m_handle;
}

std::string Shader::log() const
{
	auto logLength = GLint {};

	glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);

	auto log = std::string(logLength, 0);

	glGetShaderInfoLog(m_handle, logLength, nullptr, log.data());

	return log;
}

void Shader::source(std::span<const std::byte> source_)
{
	const auto text = reinterpret_cast<const GLchar *>(source_.data());
	const auto length = static_cast<GLint>(source_.size());

	glShaderSource(m_handle, 1, &text, &length);
}

bool Shader::compile()
{
	auto status = GLint {};

	glCompileShader(m_handle);
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);

	return status == GL_TRUE;
}

void Shader::swap(Shader &other) noexcept
{
	std::swap(m_type, other.m_type);
	std::swap(m_handle, other.m_handle);
}

void Shader::reset() noexcept
{
	if (m_handle)
	{
		glDeleteShader(m_handle);

		m_type = {};
		m_handle = {};
	}
}

} // namespace dust::gles3