#pragma once

#include <cstdint>

namespace dust::scene
{

struct Mesh
{
	int32_t faceOffset, faceCount, positionOffset, texCoordsOffset, material;
};

} // namespace dust::scene