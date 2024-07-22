#pragma once

#include <dust/scene/skybox.hpp>

namespace dust::scene
{

class Scene
{
public:
	Scene() = default;
	Scene(const Scene &) = delete;

	Scene &operator=(const Scene &) = delete;

	void skybox(
		glm::vec3 size, std::shared_ptr<gles3::ShaderProgram> shaderProgram, std::shared_ptr<gles3::Texture> cubeMap);

	void present(bool drawSkybox = true) const;

private:
	std::unique_ptr<Skybox> m_skybox;
};

} // namespace dust::scene