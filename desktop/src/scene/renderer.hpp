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

	void draw(std::span<const RenderList> lists) const;
	void view(const glm::mat4 &projection, const glm::mat4 &view);

private:
	struct InstanceInfo
	{
		glm::mat4 transform;
		int32_t positionOffset, texCoordOffset, normalOffset;
	};

	static constexpr auto POSITION_BUFFER_BINDING = 0;
	static constexpr auto TEXCOORD_BUFFER_BINDING = 1;
	static constexpr auto NORMAL_BUFFER_BINDING = 2;
	static constexpr auto INSTANCE_BUFFER_BINDING = 6;

	void initMaterials(std::span<const Material> materials);

	[[nodiscard]] static std::vector<gles3::Shader> compileShaders(
		const std::vector<std::pair<GLenum, std::filesystem::path>> &shaderPath);
	[[nodiscard]] static gles3::ShaderProgram loadShaderProgram(const std::vector<gles3::Shader> &shaders);

	gles3::Buffer m_positionsBuffer, m_texCoordBuffer, m_normalBuffer, m_facesBuffer;
	gles3::Buffer m_viewUbo;
	mutable gles3::Buffer m_instanceBuffer;
	mutable int64_t m_instanceBufferSize;
	mutable std::vector<InstanceInfo> m_instanceCache;
	std::vector<std::shared_ptr<gles3::ShaderProgram>> m_shaderPrograms;
};

} // namespace dust::scene