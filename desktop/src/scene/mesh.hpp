#pragma once

#include <cstdint>

namespace dust::scene
{

struct Mesh
{
	int64_t offset, count; //	To the faces array.
	int32_t material;
};

} // namespace dust::scene