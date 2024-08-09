#pragma once

#include <glm/glm.hpp>

namespace dust::scene
{

struct Plane
{
	Plane() = default;
	Plane(glm::vec3 normal_, float d_);

	[[nodiscard]] float distance(glm::vec3 point) const;

	glm::vec3 normal;
	float d;
};

} // namespace dust::scene