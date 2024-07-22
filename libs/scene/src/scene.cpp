#include <dust/scene/scene.hpp>

namespace dust::scene
{

void Scene::skybox(
	glm::vec3 size, std::shared_ptr<gles3::ShaderProgram> shaderProgram, std::shared_ptr<gles3::Texture> cubeMap)
{
	m_skybox = std::make_unique<Skybox>(size, std::move(shaderProgram), std::move(cubeMap));
}

void Scene::present(bool drawSkybox) const
{
	if (m_skybox and drawSkybox)
	{
		m_skybox->present();
	}
}

} // namespace dust::scene