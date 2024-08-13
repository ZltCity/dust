#include <ranges>

#include "../util/blob.hpp"
#include "renderer.hpp"

namespace dust::scene
{

Renderer::Renderer(const Scene &scene)
	: m_positionsBuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW, std::span(scene.positions())),
	  m_texCoordBuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW, std::span(scene.texCoords())),
	  m_normalBuffer(GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW, std::span(scene.normals())),
	  m_facesBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, std::span(scene.faces())),
	  m_viewUbo(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, sizeof(glm::mat4) * 2),
	  m_instanceBufferSize {}
{
	initMaterials(scene.materials());
}

void Renderer::draw(std::span<const RenderList> lists) const
{
	auto facesBinding = gles3::BindGuard(m_facesBuffer);

	gles3::Binding(m_positionsBuffer, POSITION_BUFFER_BINDING).bindBase();
	gles3::Binding(m_texCoordBuffer, TEXCOORD_BUFFER_BINDING).bindBase();
	gles3::Binding(m_normalBuffer, NORMAL_BUFFER_BINDING).bindBase();

	for (const auto &ls : lists)
	{
		const auto &shaderProgram = m_shaderPrograms.at(ls.material);

		shaderProgram->use();

		gles3::Binding(m_viewUbo, shaderProgram->uniformBlockIndex("View")).bindBase();

		m_instanceCache.clear();

		for (const auto &batch : ls.batches)
		{
			m_instanceCache.emplace_back(
				batch.transform, batch.positionOffset, batch.texCoordOffset, batch.normalOffset);
		}

		if (m_instanceBufferSize < static_cast<int32_t>(m_instanceCache.size()))
		{
			m_instanceBuffer = gles3::Buffer(
				GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW, std::span<const InstanceInfo>(m_instanceCache));
			m_instanceBufferSize = static_cast<int32_t>(m_instanceCache.size());
		}
		else
		{
			m_instanceBuffer.subData(0, std::span<const InstanceInfo>(m_instanceCache));
		}

		gles3::Binding(m_instanceBuffer, INSTANCE_BUFFER_BINDING).bindBase();

		auto drawID = int32_t {};

		for (const auto &batch : ls.batches)
		{
			shaderProgram->uniform("drawID", drawID++);
			glDrawElements(
				GL_TRIANGLES, batch.faceCount * 3, GL_UNSIGNED_INT,
				reinterpret_cast<void *>(batch.faceOffset * sizeof(Face)));
		}
	}
}

void Renderer::view(const glm::mat4 &projection, const glm::mat4 &view)
{
	m_viewUbo.update(0, projection, view);
}

void Renderer::initMaterials(std::span<const Material> materials)
{
	auto basicProgram = std::make_shared<gles3::ShaderProgram>(loadShaderProgram(compileShaders({
		std::make_pair(GL_VERTEX_SHADER, "./assets/shaders/basic.vs"),
		std::make_pair(GL_FRAGMENT_SHADER, "./assets/shaders/basic.fs"),
		//		std::make_pair(GL_GEOMETRY_SHADER, "./assets/shaders/wireframe.gs"),
	})));

	for (const auto &m : materials)
	{
		m_shaderPrograms.push_back(basicProgram);
	}
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