#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../gles3/buffer.hpp"
#include "../gles3/shader_program.hpp"

namespace dust::rendering
{

struct Brush
{
	void set() const;

	std::shared_ptr<gles3::ShaderProgram> shaderProgram;
	std::vector<std::pair<std::string, std::shared_ptr<gles3::Buffer>>> ubo;
};

} // namespace dust::rendering