#pragma once

#include <filesystem>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "../gles3/buffer.hpp"
#include "../gles3/shader_program.hpp"
#include "../gles3/vertex_array.hpp"
#include "bvh.hpp"
#include "face.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "camera.hpp"

namespace dust::scene
{

class Scene
{
public:
	Scene(std::filesystem::path assetsDir, const std::string &mapName);
	Scene(const Scene &) = delete;

	Scene &operator=(const Scene &) = delete;

	void drawStaticGeometry(float xrel, float yrel) const;

private:
	void loadMap(const std::string &mapName);
	void loadMapGeometry(const std::filesystem::path &filePath);
	void buildStaticBVH();
	void importMaterial(const aiMaterial *material);

	const std::filesystem::path m_assetsDir;

	mutable game::Camera m_camera;

	std::vector<scene::Material> m_materials;

	struct
	{
		std::vector<glm::vec3> vertices;
		std::vector<Face> faces;
	} m_staticGeometry;

	std::vector<Mesh> m_meshes;
	std::vector<std::vector<BVH>> m_staticBvh;
	mutable gles3::Buffer m_staticVbo, m_staticUbo;
	gles3::VertexArray m_staticVao;
	gles3::ShaderProgram m_shaderProgram;
	int64_t m_staticVerticesCount;
};

} // namespace dust::scene
