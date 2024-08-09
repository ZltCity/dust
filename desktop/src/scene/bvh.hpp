#pragma once

#include <span>
#include <vector>

#include "aabb.hpp"
#include "face.hpp"

namespace dust::scene
{

struct BVH
{
	[[nodiscard]] static std::vector<BVH> build(std::span<glm::vec3> vertices, std::span<Face> faces);

	AABB aabb;
	int32_t next;
};

} // namespace dust::scene
