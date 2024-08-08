#include <ranges>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "../util/stream.hpp"
#include "bvh.hpp"
#include "scene.hpp"

namespace dust::scene
{

Scene::Scene(std::filesystem::path assetsDir, const std::string &mapName) : m_assetsDir(std::move(assetsDir))
{
	loadMap(mapName);
}

void Scene::loadMap(const std::string &mapName)
{
	//	auto json = nlohmann::json::parse(util::openStream(m_assetsDir / "maps" / (mapName + ".tmj"),
	// std::ios_base::in));
	//
	//	for (const auto &layer : json.at("layers"))
	//	{
	//		if (layer.at("class").get<std::string>() == "plan")
	//		{
	//			for (const auto &properties : layer.at("properties"))
	//			{
	//				if (properties.at("name").get<std::string>() == "mesh")
	//				{
	//					loadMapGeometry(m_assetsDir / "maps" / properties.at("value").get<std::string>());
	//				}
	//			}
	//		}
	//	}
	loadMapGeometry("./assets/maps/test/buddha.obj");
}

void Scene::loadMapGeometry(const std::filesystem::path &glbFile)
{
	auto importer = Assimp::Importer();
	const auto scene = importer.ReadFile(
		(glbFile).generic_string(),
		aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (not scene)
	{
		throw std::runtime_error(
			fmt::format("Could not import model file '{}'\n{}.", glbFile.generic_string(), importer.GetErrorString()));
	}

	//	Import materials.
	for (auto i = 0; i < scene->mNumMaterials; ++i)
	{
		importMaterial(scene->mMaterials[i]);
	}

	//	Copy geometry;
	auto meshGroups = std::vector<std::vector<int32_t>>(scene->mNumMaterials);
	auto verticesCount = int64_t {}, facesCount = int64_t {};

	for (auto i = 0; i < scene->mNumMeshes; ++i)
	{
		const auto mesh = scene->mMeshes[i];

		meshGroups.at(mesh->mMaterialIndex).push_back(i);

		verticesCount += mesh->mNumVertices;
		facesCount += mesh->mNumFaces;
	}

	m_staticGeometry.vertices.reserve(verticesCount);
	m_staticGeometry.faces.reserve(facesCount);

	for (auto i = 0; i < scene->mNumMaterials; ++i)
	{
		for (auto mi : meshGroups.at(i))
		{
			const auto faceOffset = m_staticGeometry.faces.size();
			const auto mesh = scene->mMeshes[mi];

			std::ranges::transform(
				std::span(mesh->mVertices, mesh->mNumVertices), std::back_inserter(m_staticGeometry.vertices),
				[](const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); });
			std::ranges::transform(
				std::span(mesh->mFaces, mesh->mNumFaces), std::back_inserter(m_staticGeometry.faces),
				[](const aiFace &f) {
					return Face {
						.v1 = static_cast<int32_t>(f.mIndices[0]),
						.v2 = static_cast<int32_t>(f.mIndices[1]),
						.v3 = static_cast<int32_t>(f.mIndices[2]),
					};
				});

			m_meshes.emplace_back(
				static_cast<int64_t>(faceOffset), static_cast<int64_t>(mesh->mNumFaces), static_cast<int32_t>(i));
		}
	}

	buildStaticBVH();
}

void Scene::buildStaticBVH()
{
	struct Batch
	{
		int64_t offset, size;
	};

	auto batches = std::vector<Batch> {Batch {.offset = {}, .size = {}}};
	auto lastMaterial = int32_t {};
	auto bvh = std::vector<std::vector<BVH>> {};

	for (const auto &mesh : m_meshes)
	{
		auto &batch = batches.back();

		if (mesh.material != lastMaterial)
		{
			batch = batches.emplace_back(batch.offset + batch.size, 0);
			lastMaterial = mesh.material;
		}

		batch.size += mesh.count;
	}

	for (const auto &batch : batches)
	{
		bvh.push_back(BVH::build(
			std::span(m_staticGeometry.vertices),
			std::span(m_staticGeometry.faces.begin() + batch.offset, batch.size)));
	}
}

void Scene::importMaterial(const aiMaterial *material)
{
	m_materials.push_back({});
}

} // namespace dust::scene