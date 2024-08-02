#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"

namespace dust::game
{

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	: m_position(position), m_forward(target - position), m_up(up), m_yawAngle {}, m_pitchAngle {}
{}

void Camera::yaw(float angle)
{
	m_yawAngle += angle;
}

void Camera::pitch(float angle)
{
	m_pitchAngle = glm::clamp(m_pitchAngle + angle, -pitchLimit, pitchLimit);
}

void Camera::move(const glm::vec3 &direction)
{
	m_position += direction;
}

glm::vec3 Camera::position() const
{
	return m_position;
}

void Camera::position(glm::vec3 value)
{
	m_position = value;
}

Axes Camera::axes() const
{
	auto forward = glm::vec3(glm::rotate(glm::mat4(1.f), m_yawAngle, m_up) * glm::vec4(m_forward, 0.f));
	auto right = glm::cross(forward, m_up);

	forward = glm::rotate(glm::mat4(1.f), m_pitchAngle, right) * glm::vec4(forward, 0.f);

	return {glm::normalize(right), glm::normalize(glm::cross(right, forward)), glm::normalize(forward)};
}

glm::mat4 Camera::view() const
{
	const auto [x, y, z] = axes();

	return glm::lookAtRH(m_position, m_position + z, y);
}

glm::mat4 Camera::ortho(float left, float right, float bottom, float top, float near, float far)
{
	return glm::orthoRH(left, right, bottom, top, near, far);
}

glm::mat4 Camera::perspective(float fovY, float aspect, float near, float far)
{
	return glm::perspectiveRH(fovY, aspect, near, far);
}

} // namespace dust::game