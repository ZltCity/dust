#pragma once

#include <vector>

#include "../gles3/buffer.hpp"
#include "../gles3/shader_program.hpp"
#include "../scene/scene.hpp"

namespace dust::scene
{

class Renderer
{
public:
	explicit Renderer(const Scene &scene);
	Renderer(const Renderer &) = delete;

	Renderer &operator=(const Renderer &) = delete;

	void useMaterial(int32_t index) const;
	void drawBatch(int32_t meshIndex, int32_t faceOffset, int32_t faceCount) const;
	void projection(const glm::mat4 &m);
	void view(const glm::mat4 &m);
	void model(const glm::mat4 &m);

private:
	[[nodiscard]] static std::vector<gles3::Shader> compileShaders(
		const std::vector<std::pair<GLenum, std::filesystem::path>> &shaderPath);
	[[nodiscard]] static gles3::ShaderProgram loadShaderProgram(const std::vector<gles3::Shader> &shaders);

	std::vector<Mesh> m_meshes;
	gles3::Buffer m_positionsBuffer, m_texCoordsBuffer, m_facesBuffer, m_transformUbo;
	gles3::ShaderProgram m_basicShaderProgram;
};

} // namespace dust::scene