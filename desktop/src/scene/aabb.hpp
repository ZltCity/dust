#pragma once

#include <glm/glm.hpp>

namespace dust::scene
{

struct AABB
{
	AABB();
	AABB(glm::vec3 min, glm::vec3 max);
	AABB(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	void extend(glm::vec3 v);

	glm::vec3 min, max;
};

} // namespace dust::scene