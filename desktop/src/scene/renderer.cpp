#include <ranges>

#include "../util/blob.hpp"
#include "renderer.hpp"

namespace dust::scene
{

Renderer::Renderer(const Scene &scene)
	: m_meshes(scene.meshes()),
	  m_positionsBuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW, std::span(scene.positions())),
	  m_texCoordsBuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW, std::span(scene.texCoords())),
	  m_facesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, std::span(scene.faces())),
	  m_transformUbo(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, sizeof(glm::mat4) * 3),
	  m_basicShaderProgram(loadShaderProgram(compileShaders({
		  std::make_pair(GL_VERTEX_SHADER, "./assets/shaders/basic.vs"),
		  std::make_pair(GL_FRAGMENT_SHADER, "./assets/shaders/basic.fs"),
		  std::make_pair(GL_GEOMETRY_SHADER, "./assets/shaders/wireframe.gs"),
	  })))
{}

void Renderer::useMaterial(int32_t index) const
{
	m_basicShaderProgram.use();

	gles3::Binding(m_transformUbo, m_basicShaderProgram.uniformBlockIndex("Transform")).bindBase();
}

void Renderer::drawBatch(int32_t meshIndex, int32_t faceOffset, int32_t faceCount) const
{
	auto facesBinding = gles3::BindGuard(m_facesBuffer);

	gles3::Binding(m_positionsBuffer, 0).bindBase();
	gles3::Binding(m_texCoordsBuffer, 1).bindBase();

	glDrawElements(
		GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, reinterpret_cast<void *>(faceOffset * 3 * sizeof(Face)));
}

void Renderer::projection(const glm::mat4 &m)
{
	m_transformUbo.update(0, m);
}

void Renderer::view(const glm::mat4 &m)
{
	m_transformUbo.update(sizeof(glm::mat4), m);
}

void Renderer::model(const glm::mat4 &m)
{
	m_transformUbo.update(sizeof(glm::mat4) * 2, m);
}

std::vector<gles3::Shader> Renderer::compileShaders(
	const std::vector<std::pair<GLenum, std::filesystem::path>> &shaderPaths)
{
	auto shaders = std::vector<gles3::Shader> {};

	std::ranges::transform(shaderPaths, std::back_inserter(shaders), [](auto &&shaderPath) {
		const auto &[type, path] = shaderPath;
		auto shader = gles3::Shader(type, util::loadBlob(path));

		if (not shader.compile())
		{
			throw std::runtime_error(shader.log());
		}

		return shader;
	});

	return shaders;
}

gles3::ShaderProgram Renderer::loadShaderProgram(const std::vector<gles3::Shader> &shaders)
{
	auto program = gles3::ShaderProgram(shaders);

	if (not program.link())
	{
		throw std::runtime_error(program.log());
	}

	return program;
}

} // namespace dust::scene