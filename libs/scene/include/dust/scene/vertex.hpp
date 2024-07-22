#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <dust/gles3/vertex_array.hpp>

namespace dust::scene
{

struct BasicVertex
{
	[[nodiscard]] static std::vector<gles3::VertexAttrib> attributes();

	glm::vec3 position;
	glm::vec2 texCoord = {0.f, 0.f};
};

} // namespace dust::scene