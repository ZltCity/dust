#pragma once

#include <vector>

#include "shader.hpp"

namespace dust::editor::reflections
{

struct Brush
{
	std::string name;
	std::vector<Shader> shaders = {};
};

} // namespace dust::editor::reflections