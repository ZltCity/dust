#pragma once

#include <glm/glm.hpp>

namespace dust::game
{

struct Axes
{
	glm::vec3 x, y, z;
};

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up = {0.f, 1.f, 0.f});

	void yaw(float angle);
	void pitch(float angle);
	void move(const glm::vec3 &direction);

	[[nodiscard]] glm::vec3 position() const;
	void position(glm::vec3 value);

	[[nodiscard]] Axes axes() const;
	[[nodiscard]] glm::mat4 view() const;

	[[nodiscard]] static glm::mat4 ortho(float left, float right, float bottom, float top, float near, float far);
	[[nodiscard]] static glm::mat4 perspective(float fovY, float aspect, float near, float far);

private:
	static constexpr auto pitchLimit = glm::radians(90.f);

	[[nodiscard]] float calcPitch(const glm::vec3 &forward) const;

	const glm::vec3 m_forward, m_up;
	const float m_pitchCorrection;

	glm::vec3 m_position;
	float m_yawAngle, m_pitchAngle;
};

} // namespace dust::game