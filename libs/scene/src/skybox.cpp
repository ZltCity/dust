#include <dust/scene/shapes/cube.hpp>
#include <dust/scene/skybox.hpp>

namespace dust::scene
{

Skybox::Skybox(
	glm::vec3 size, std::shared_ptr<gles3::ShaderProgram> shaderProgram, std::shared_ptr<gles3::Texture> cubeMap)
	: m_size(size)
{
	const auto cube = shapes::Cube<BasicVertex>();
	const auto faces = cube.faces();

	m_mesh = Mesh {
		.vbo = std::make_shared<gles3::Buffer>(GL_ARRAY_BUFFER, cube.vertices(), GL_STATIC_DRAW),
		.ibo = std::make_shared<gles3::Buffer>(GL_ELEMENT_ARRAY_BUFFER, faces, GL_STATIC_DRAW),
		.vao = std::make_shared<gles3::VertexArray>(BasicVertex::attributes()),
		.drawMode = GL_TRIANGLES,
		.count = static_cast<GLsizei>(faces.size()),
		.offset = 0,
	};
	m_brush = Brush {
		.shaderProgram = std::move(shaderProgram),
		.ubo = std::make_shared<gles3::Buffer>(GL_UNIFORM_BUFFER),
		.textures = {std::move(cubeMap)},
	};
}

void Skybox::present() const
{}

} // namespace dust::scene