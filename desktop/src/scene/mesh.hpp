#pragma once

#include <cstdint>

namespace dust::scene
{

struct Mesh
{
	int32_t faceOffset, faceCount;
	int32_t positionOffset, texCoordOffset, normalOffset;
	int32_t material;
};

} // namespace dust::scene