#pragma once

#include <span>
#include <vector>

#include "aabb.hpp"
#include "face.hpp"

namespace dust::scene
{

struct BVH
{
	[[nodiscard]] static std::vector<BVH> build(std::span<const glm::vec3> vertices, std::span<const Face> faces);

	AABB aabb;
	int32_t batchIndex, childrenCount, children[];
};

} // namespace dust::scene
