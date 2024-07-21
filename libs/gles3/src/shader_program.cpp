#include <dust/gles3/shader_program.hpp>

namespace dust::gles3
{

ShaderProgram::ShaderProgram() : m_handle {}
{}

ShaderProgram::ShaderProgram(std::span<const Shader> shaders) : m_handle(glCreateProgram())
{
	for (const auto &s : shaders)
	{
		attach(s);
	}
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept : m_handle {}
{
	swap(other);
}

ShaderProgram::~ShaderProgram() noexcept
{
	reset();
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

ShaderProgram::operator bool() const
{
	return m_handle > 0;
}

GLuint ShaderProgram::handle() const
{
	return m_handle;
}

std::string ShaderProgram::log() const
{
	auto logLength = GLint {};

	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);

	auto log = std::string(logLength, 0);

	glGetProgramInfoLog(m_handle, logLength, nullptr, log.data());

	return log;
}

void ShaderProgram::attach(const Shader &shader)
{
	glAttachShader(m_handle, shader.handle());
}

bool ShaderProgram::link()
{
	auto status = GLint {};

	glLinkProgram(m_handle);
	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);

	return status == GL_TRUE;
}

void ShaderProgram::swap(ShaderProgram &other) noexcept
{
	std::swap(m_handle, other.m_handle);
}

void ShaderProgram::reset() noexcept
{
	if (m_handle)
	{
		glDeleteProgram(m_handle);

		m_handle = {};
	}
}

} // namespace dust::gles3