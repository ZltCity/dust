#include <ranges>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "../gles3/bind_guard.hpp"
#include "../util/blob.hpp"
#include "../util/stream.hpp"
#include "scene.hpp"

namespace dust::scene
{

Scene::Scene(std::filesystem::path assetsDir, const std::string &mapName)
	: m_assetsDir(std::move(assetsDir)), m_camera(glm::vec3(0.f, 0.0f, 2.f), glm::vec3(0.f))
{
	loadMap(mapName);
}

void Scene::drawStaticGeometry(float xrel, float yrel) const
{
	m_camera.yaw(xrel);
	m_camera.pitch(yrel);
	m_shaderProgram.use();
	m_staticUbo.update(
		0, game::Camera::perspective(glm::radians(95.f), 16.f / 9.f, 0.1f, 1000.f),
		m_camera.view() // * glm::rotate(glm::mat4(1.f), glm::radians(270.f), glm::vec3(1.f, 0.f, 0.f)) *
			* glm::scale(glm::mat4(1.f), glm::vec3(5.f))
		);
	gles3::Binding(m_staticUbo, m_shaderProgram.uniformBlockIndex("Transform")).bindBase();
	auto vaoBind = gles3::BindGuard(m_staticVao);

	glDrawArrays(GL_LINES, 0, m_staticVerticesCount);
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

void Scene::loadMapGeometry(const std::filesystem::path &filePath)
{
	auto importer = Assimp::Importer();
	const auto scene = importer.ReadFile(
		filePath.generic_string(),
		aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	if (not scene)
	{
		throw std::runtime_error(
			fmt::format("Could not import model file '{}'\n{}.", filePath.generic_string(), importer.GetErrorString()));
	}

	//	Import materials.
	for (auto i = 0u; i < scene->mNumMaterials; ++i)
	{
		importMaterial(scene->mMaterials[i]);
	}

	//	Copy geometry;
	auto meshGroups = std::vector<std::vector<int32_t>>(scene->mNumMaterials);
	auto verticesCount = int64_t {}, facesCount = int64_t {};

	for (auto i = 0u; i < scene->mNumMeshes; ++i)
	{
		const auto mesh = scene->mMeshes[i];

		meshGroups.at(mesh->mMaterialIndex).push_back(i);

		verticesCount += mesh->mNumVertices;
		facesCount += mesh->mNumFaces;
	}

	m_staticGeometry.vertices.reserve(verticesCount);
	m_staticGeometry.faces.reserve(facesCount);

	for (auto i = 0u; i < scene->mNumMaterials; ++i)
	{
		for (auto mi : meshGroups.at(i))
		{
			const auto vertexOffset = m_staticGeometry.vertices.size(), faceOffset = m_staticGeometry.faces.size();
			const auto mesh = scene->mMeshes[mi];

			std::ranges::transform(
				std::span(mesh->mVertices, mesh->mNumVertices), std::back_inserter(m_staticGeometry.vertices),
				[](const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); });
			std::ranges::transform(
				std::span(mesh->mFaces, mesh->mNumFaces), std::back_inserter(m_staticGeometry.faces),
				[vertexOffset](const aiFace &f) {
					return Face {
						.v1 = static_cast<int32_t>(vertexOffset + f.mIndices[0]),
						.v2 = static_cast<int32_t>(vertexOffset + f.mIndices[1]),
						.v3 = static_cast<int32_t>(vertexOffset + f.mIndices[2]),
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
		m_staticBvh.push_back(BVH::build(
			std::span(m_staticGeometry.vertices),
			std::span(m_staticGeometry.faces.begin() + batch.offset, batch.size)));
	}

	auto boxes = std::vector<glm::vec3> {};

	for (const auto &tree : m_staticBvh)
	{
		for (const auto &node : tree)
		{
			const auto boxSize = node.aabb.size();
			const auto v1 = node.aabb.min, v2 = v1 + glm::vec3(boxSize.x, 0.f, 0.f),
					   v3 = v1 + glm::vec3(0.f, boxSize.y, 0.f), v4 = v1 + glm::vec3(0.f, 0.f, boxSize.z),
					   v5 = node.aabb.max, v6 = v5 - glm::vec3(boxSize.x, 0.f, 0.f),
					   v7 = v5 - glm::vec3(0.f, boxSize.y, 0.f), v8 = v5 - glm::vec3(0.f, 0.f, boxSize.z);
			const auto lines = std::vector<glm::vec3> {v1, v2, v2, v7, v7, v4, v4, v1, v1, v3, v2, v8,
													   v7, v5, v4, v6, v3, v8, v8, v5, v5, v6, v6, v3};

			boxes.insert(boxes.end(), lines.begin(), lines.end());
		}
	}

	m_staticVerticesCount = boxes.size();

	m_staticVbo = gles3::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, boxes);
	m_staticUbo = gles3::Buffer(GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, static_cast<GLsizeiptr>(sizeof(glm::mat4) * 2));

	auto shaders = std::array {
		gles3::Shader(GL_VERTEX_SHADER, util::loadBlob("./assets/shaders/basic.vs")),
		gles3::Shader(GL_FRAGMENT_SHADER, util::loadBlob("./assets/shaders/basic.fs")),
		//		gles3::Shader(GL_GEOMETRY_SHADER, util::loadBlob("./assets/shaders/wireframe.gs")),
	};

	for (auto &shader : shaders)
	{
		if (not shader.compile())
		{
			throw std::runtime_error(shader.log());
		}
	}

	m_shaderProgram = gles3::ShaderProgram(shaders);

	if (not m_shaderProgram.link())
	{
		throw std::runtime_error(m_shaderProgram.log());
	}

	const auto vertexAttribs = std::array {
		gles3::VertexAttrib {
			.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(glm::vec3), .offset = 0},
		//		gles3::VertexAttrib {
		//			.size = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(Vertex), .offset =
		//sizeof(glm::vec3)},
	};

	m_staticVao = gles3::VertexArray(m_staticVbo, vertexAttribs);
}

void Scene::importMaterial(const aiMaterial *material)
{
	m_materials.push_back({});
}

} // namespace dust::scene