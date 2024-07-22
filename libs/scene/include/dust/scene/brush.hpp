#pragma once

#include <memory>
#include <vector>

#include <dust/gles3/buffer.hpp>
#include <dust/gles3/shader_program.hpp>
#include <dust/gles3/texture.hpp>

namespace dust::scene
{

struct Brush
{
	std::shared_ptr<gles3::ShaderProgram> shaderProgram;
	std::shared_ptr<gles3::Buffer> ubo;
	std::vector<std::shared_ptr<gles3::Texture>> textures;
};

} // namespace dust::scene