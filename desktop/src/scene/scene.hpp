#pragma once

#include <filesystem>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "bvh.hpp"
#include "camera.hpp"
#include "face.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "render_list.hpp"

namespace dust::scene
{

class Scene
{
public:
	Scene(std::filesystem::path assetsDir, const std::string &mapName);
	Scene(const Scene &) = delete;

	Scene &operator=(const Scene &) = delete;

	[[nodiscard]] Camera &camera();
	[[nodiscard]] const Camera &camera() const;

	[[nodiscard]] const std::vector<Material> &materials() const;
	[[nodiscard]] const std::vector<Mesh> &meshes() const;
	[[nodiscard]] const std::vector<glm::vec3> &positions() const;
	[[nodiscard]] const std::vector<glm::vec2> &texCoords() const;
	[[nodiscard]] const std::vector<glm::vec3> &normals() const;
	[[nodiscard]] const std::vector<Face> &faces() const;

	[[nodiscard]] std::vector<RenderList> present() const;

private:
	void loadMap(const std::string &mapName);
	[[nodiscard]] std::pair<int32_t, int32_t> importModelFile(const std::filesystem::path &filePath);
	void importMaterial(const aiMaterial *material);
	void importMesh(const aiMesh *mesh, int32_t materialOffset);
	void buildStaticBVH(int32_t meshOffset, int32_t meshCount);

	[[nodiscard]] int32_t baseMaterialOffset() const;
	[[nodiscard]] int32_t baseMeshOffset() const;
	[[nodiscard]] int32_t basePositionOffset() const;
	[[nodiscard]] int32_t baseTexCoordOffset() const;
	[[nodiscard]] int32_t baseNormalOffset() const;
	[[nodiscard]] int32_t baseFaceOffset() const;

	const std::filesystem::path m_assetsDir;

	mutable Camera m_camera;

	struct
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<Face> faces;
	} m_geometryCache;

	std::vector<Material> m_materials;
	std::vector<Mesh> m_meshes;
};

} // namespace dust::scene
