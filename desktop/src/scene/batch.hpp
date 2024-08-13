#pragma once

#include <glm/glm.hpp>

namespace dust::scene
{

struct Batch
{
	glm::mat4 transform;
	int32_t material, mesh, faceOffset, faceCount;
};

} // namespace dust::scene