#include <numeric>

#include "aabb.hpp"

namespace dust::scene
{

AABB::AABB() : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::lowest())
{}

AABB::AABB(glm::vec3 min_, glm::vec3 max_) : min(min_), max(max_)
{}

AABB::AABB(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
	: min(glm::min(v1, glm::min(v2, v3))), max(glm::max(v1, glm::max(v2, v3)))
{}

glm::vec3 AABB::size() const
{
	return max - min;
}

std::pair<AABB, AABB> AABB::split(glm::vec3 axis, float factor) const
{
	const auto d = axis * size();

	return {AABB(min, max - d * (1.f - factor)), AABB(min + d * factor, max)};
}

void AABB::extend(glm::vec3 v)
{
	min = glm::min(min, v);
	max = glm::max(max, v);
}

} // namespace dust::scene