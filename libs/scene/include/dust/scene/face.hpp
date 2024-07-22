#pragma once

#include <cstdint>

namespace dust::scene
{

struct BasicFace
{
	union {
		struct
		{
			uint32_t v1, v2, v3;
		};
		uint32_t v[3];
	};
};

} // namespace dust::scene