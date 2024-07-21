#include <glm/gtc/matrix_transform.hpp>

#include <dust/scene/camera.hpp>

namespace dust::scene
{

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	: m_position(), m_forward(), m_up(), m_right(), m_projection(1.f)
{
	lookAt(position, target, up);
}

void Camera::yaw(float angle, const glm::vec3 &up)
{
	m_forward = glm::rotate(glm::mat4(1.f), angle, up) * glm::vec4(m_forward, 0.f);
	m_right = glm::cross(m_forward, up);
	m_up = glm::cross(m_forward, m_right);
}

void Camera::yaw(float angle, const glm::vec3 &pivot, const glm::vec3 &up)
{}

void Camera::pitch(float angle)
{
	m_forward = glm::rotate(glm::mat4(1.f), angle, m_right) * glm::vec4(m_forward, 0.f);
	m_up = glm::cross(m_forward, m_right);
//	m_right = glm::cross(m_forward, m_up);
}

void Camera::pitch(float angle, const glm::vec3 &pivot)
{}

void Camera::roll(float angle)
{
	m_up = glm::rotate(glm::mat4(1.f), angle, m_forward) * glm::vec4(m_up, 0.f);
	m_right = glm::cross(m_forward, m_up);
//	m_up = glm::cross(m_forward, m_right);
}

void Camera::move(const glm::vec3 &distance)
{
	m_position += distance;
}

void Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	m_position = position;
//	m_target = target;
	m_forward = target - m_position;
	m_up = up;
	m_right = glm::cross(m_forward, m_up);
}

void Camera::ortho(float left, float right, float bottom, float top, float near, float far)
{
	m_projection = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::perspective(float fovY, float aspect, float near, float far)
{
	m_projection = glm::perspective(fovY, aspect, near, far);
}

glm::vec3 Camera::position() const
{
	return m_position;
}

//glm::vec3 Camera::target() const
//{
//	return m_target;
//}

glm::vec3 Camera::forward() const
{
	return m_forward;
}

glm::vec3 Camera::up() const
{
	return m_up;
}

glm::vec3 Camera::right() const
{
	return m_right;
}

glm::mat4 Camera::view() const
{
	m_forward = glm::normalize(m_forward);
	m_up = glm::normalize(m_up);
//	m_right = glm::normalize(m_right);

	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera::projection() const
{
	return m_projection;
}

} // namespace dust::scene