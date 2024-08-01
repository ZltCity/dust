#pragma once

#include <memory>

#include "../gles3/shader_program.hpp"

namespace dust::rendering
{

struct Brush
{
	void set() const;

	std::shared_ptr<gles3::ShaderProgram> shaderProgram;
};

} // namespace dust::rendering