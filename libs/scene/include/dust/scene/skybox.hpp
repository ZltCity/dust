#pragma once

#include <glm/glm.hpp>

#include <dust/gles3/texture.hpp>
#include <dust/scene/brush.hpp>
#include <dust/scene/drawable.hpp>
#include <dust/scene/mesh.hpp>

namespace dust::scene
{

class Skybox final : public Drawable
{
public:
	Skybox(
		glm::vec3 size, std::shared_ptr<gles3::ShaderProgram> shaderProgram, std::shared_ptr<gles3::Texture> cubeMap);

	void present() const final;

private:
	glm::vec3 m_size;
	Mesh m_mesh;
	Brush m_brush;
};

} // namespace dust::scene