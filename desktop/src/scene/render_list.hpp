#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace dust::scene
{

struct RenderList
{
	struct Batch
	{
		glm::mat4 transform;
		int32_t faceOffset, faceCount, positionOffset, texCoordOffset, normalOffset;
	};

	int32_t material;

	std::vector<Batch> batches;
};

} // namespace dust::scene