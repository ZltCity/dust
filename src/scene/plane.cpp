#include "plane.hpp"

namespace dust::scene
{

Plane::Plane(glm::vec3 normal_, float d_) : normal(normal_), d(d_)
{}

float Plane::distance(glm::vec3 point) const
{
	return glm::dot(normal, point) - d;
}

} // namespace dust::scene