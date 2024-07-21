#pragma once

#include <glm/glm.hpp>

namespace dust::scene
{

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up = {0.f, 1.f, 0.f});

	void yaw(float angle, const glm::vec3 &up);
	void yaw(float angle, const glm::vec3 &pivot, const glm::vec3 &up);
	void pitch(float angle);
	void pitch(float angle, const glm::vec3 &pivot);
	void roll(float angle);
	void move(const glm::vec3 &distance);
	void lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 up = {0.1f, 1.f, 0.f});
	void ortho(float left, float right, float bottom, float top, float near, float far);
	void perspective(float fovY, float aspect, float near, float far);

	[[nodiscard]] glm::vec3 position() const;
//	[[nodiscard]] glm::vec3 target() const;
	[[nodiscard]] glm::vec3 forward() const;
	[[nodiscard]] glm::vec3 up() const;
	[[nodiscard]] glm::vec3 right() const;
	[[nodiscard]] glm::mat4 view() const;
	[[nodiscard]] glm::mat4 projection() const;

private:
	mutable glm::vec3 m_position, m_forward, m_up, m_right;
	glm::mat4 m_projection;
};

} // namespace dust::scene