#include <algorithm>
#include <ranges>

#include <fmt/format.h>

#include "bvh.hpp"

namespace dust::scene
{

void split(const AABB &parentBox, std::span<const glm::vec3> vertices, std::span<const Face> faces)
{
	const auto boxSize = parentBox.max - parentBox.min;
	struct
	{
		glm::vec3 point, normal;
	} plane {};

	if (boxSize.x >= boxSize.y and boxSize.x >= boxSize.z)
	{
		plane.point = glm::vec3(parentBox.min.x + boxSize.x / 2.f, parentBox.max.y, parentBox.max.z);
		plane.normal = glm::vec3(1.f, 0.f, 0.f);
	}
	else if (boxSize.y >= boxSize.z)
	{
		plane.point = glm::vec3(parentBox.max.x, parentBox.min.y + boxSize.y / 2.f, parentBox.max.z);
		plane.normal = glm::vec3(0.f, 1.f, 0.f);
	}
	else
	{
		plane.point = glm::vec3(parentBox.max.x, parentBox.max.y, parentBox.min.z + boxSize.z / 2.f);
		plane.normal = glm::vec3(0.f, 0.f, 1.f);
	}

	for (const auto &face : faces)
	{
		const auto center = (vertices.at(face.v1) + vertices.at(face.v2) + vertices.at(face.v3)) / 3.f;

		if (glm::dot(plane.normal, center - plane.point) > 0.f)
		{
			fmt::print("right!");
		}
		else
		{
			fmt::print("left!");
		}

	}
}

std::vector<BVH> BVH::build(std::span<const glm::vec3> vertices, std::span<const Face> faces)
{
	auto rootBox = AABB();

	std::ranges::for_each(vertices, [&rootBox](const auto &v) { rootBox.extend(v); });

	split(rootBox, vertices, faces);

	return {};
}

} // namespace dust::scene