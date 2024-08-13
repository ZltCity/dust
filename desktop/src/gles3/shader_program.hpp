#pragma once

#include "shader.hpp"

namespace dust::gles3
{

class ShaderProgram
{
public:
	ShaderProgram();
	explicit ShaderProgram(std::span<const Shader> shaders);
	ShaderProgram(const ShaderProgram &) = delete;
	ShaderProgram(ShaderProgram &&other) noexcept;
	~ShaderProgram() noexcept;

	ShaderProgram &operator=(const ShaderProgram &) = delete;
	ShaderProgram &operator=(ShaderProgram &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] GLuint handle() const;
	[[nodiscard]] std::string log() const;
	[[nodiscard]] GLuint uniformBlockIndex(const std::string &name) const;
	void uniform(const std::string &name, int32_t v);

	void attach(const Shader &shader);
	[[nodiscard]] bool link();
	void use() const;

	void swap(ShaderProgram &other) noexcept;
	void reset() noexcept;

private:
	GLuint m_handle;
};

} // namespace dust::gles3