#pragma once

#include <filesystem>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "face.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace dust::scene
{

class Scene
{
public:
	Scene(std::filesystem::path assetsDir, const std::string &mapName);
	Scene(const Scene &) = delete;

	Scene &operator=(const Scene &) = delete;

private:
	void loadMap(const std::string &mapName);
	void loadMapGeometry(const std::filesystem::path &glbFile);
	void buildStaticBVH();
	void importMaterial(const aiMaterial *material);

	const std::filesystem::path m_assetsDir;

	std::vector<scene::Material> m_materials;

	struct
	{
		std::vector<glm::vec3> vertices;
		std::vector<Face> faces;
	} m_staticGeometry;

	std::vector<Mesh> m_meshes;
};

} // namespace dust::scene
