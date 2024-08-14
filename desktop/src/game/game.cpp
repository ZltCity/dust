#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL_mouse.h>

#include "../gles3/rendering_context.hpp"
#include "../logging/log.hpp"
#include "../scene/renderer.hpp"
#include "../sdl/sdl.hpp"
#include "../util/blob.hpp"
#include "../util/stream.hpp"
#include "game.hpp"

namespace dust::game
{

using Log = logging::Log;

Game::Game(
	[[maybe_unused]] int argc, [[maybe_unused]] char **argv) //: m_camera(glm::vec3(0.f, 0.0f, 2.f), glm::vec3(0.f))
{
	initLogFile();
}

int Game::start()
{
	Log::info("Start Dust Game..");

	m_config = loadConfig(configPath);

	[[maybe_unused]] auto &sdl = sdl::Lib::instance();
	auto window = std::make_shared<sdl::Window>(
		"Dust Game", std::make_tuple(m_config.window.width, m_config.window.height),
		SDL_WINDOW_SHOWN | (m_config.window.fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	auto renderingContext = gles3::RenderingContext(window, m_config.gles3.debugContext);

	renderingContext.makeCurrent();

	if (m_config.gles3.debugContext)
	{
		glDebugMessageCallback(gles3DebugCallback, nullptr);
	}

	auto quit = false;
	auto relativeMouse = false;

	loadMap({});

	auto renderer = scene::Renderer(*m_scene);

	auto &camera = m_scene->camera();
	auto moveForward = false, moveBackward = false, moveLeft = false, moveRight = false;

	while (not quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}

			switch (event.type)
			{
				case SDL_MOUSEMOTION:
				{
					camera.yaw(static_cast<float>(event.motion.xrel) * -0.002f);
					camera.pitch(static_cast<float>(event.motion.yrel) * -0.002f);

					break;
				}
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_w: moveForward = true; break;
						case SDLK_s: moveBackward = true; break;
						case SDLK_a: moveLeft = true; break;
						case SDLK_d: moveRight = true; break;
					}
					break;
				}
				case SDL_KEYUP:
				{
					switch (event.key.keysym.scancode)
					{
						case SDL_SCANCODE_W: moveForward = false; break;
						case SDL_SCANCODE_S: moveBackward = false; break;
						case SDL_SCANCODE_A: moveLeft = false; break;
						case SDL_SCANCODE_D: moveRight = false; break;
						case SDL_SCANCODE_SPACE:
						{
							relativeMouse = !relativeMouse;
							SDL_SetRelativeMouseMode(relativeMouse ? SDL_TRUE : SDL_FALSE);
						}
					}
					break;
				}
			}
		}

		const auto [right, up, forward] = camera.axes();

		if (moveForward)
		{
			camera.move(forward * 1.5f);
		}
		if (moveBackward)
		{
			camera.move(forward * -1.5f);
		}
		if (moveLeft)
		{
			camera.move(right * -1.5f);
		}
		if (moveRight)
		{
			camera.move(right * 1.5f);
		}

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.view(scene::Camera::perspective(glm::radians(95.f), 16.f / 9.f, 0.1f, 10000.f), camera.view());
		renderer.draw(m_scene->present());

		renderingContext.swapBuffers();
	}

	saveConfig(m_config, configPath);

	return 0;
}

void Game::loadMap(const std::filesystem::path &path)
{
	Log::debug(fmt::format("Loading map '{}'.", path.generic_string()));

	m_scene = std::make_unique<scene::Scene>("./assets", "graveyard");

	//	// Create an instance of the Importer class
	//	Assimp::Importer importer;
	//
	//	// And have it read the given file with some example postprocessing
	//	// Usually - if speed is not the most important aspect for you - you'll
	//	// probably to request more postprocessing than we do in this example.
	//	const aiScene *scene = importer.ReadFile(
	//		path.generic_string(),
	//		//											 aiProcess_CalcTangentSpace |
	//		aiProcess_FlipUVs | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	//
	//	// If the import failed, report it
	//	if (nullptr == scene)
	//	{
	//		throw std::runtime_error(
	//			fmt::format("Could not import model file '{}'\n{}.", path.generic_string(), importer.GetErrorString()));
	//	}
	//
	//	auto shaders = std::array {
	//		gles3::Shader(GL_VERTEX_SHADER, util::loadBlob("./assets/shaders/basic.vs")),
	//		gles3::Shader(GL_FRAGMENT_SHADER, util::loadBlob("./assets/shaders/basic.fs")),
	//		gles3::Shader(GL_GEOMETRY_SHADER, util::loadBlob("./assets/shaders/wireframe.gs")),
	//	};
	//
	//	for (auto &shader : shaders)
	//	{
	//		if (not shader.compile())
	//		{
	//			throw std::runtime_error(shader.log());
	//		}
	//	}
	//
	//	auto shaderProgram = std::make_shared<gles3::ShaderProgram>(shaders);
	//
	//	if (not shaderProgram->link())
	//	{
	//		throw std::runtime_error(shaderProgram->log());
	//	}
	//
	//	m_transformUBO = std::make_shared<gles3::Buffer>(
	//		GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, static_cast<GLsizeiptr>(sizeof(glm::mat4) * 2));
	//	m_brushes.clear();
	//
	//	for (auto i = 0; i < scene->mNumMaterials; ++i)
	//	{
	//		const auto material = scene->mMaterials[i];
	//		auto brush =
	//			rendering::Brush {.shaderProgram = shaderProgram, .ubo = {{std::string("Transform"), m_transformUBO}}};
	//
	//		for (auto t = 0; t < material->GetTextureCount(aiTextureType_DIFFUSE); ++t)
	//		{
	//			auto texturePath = aiString();
	//
	//			if (material->GetTexture(aiTextureType_DIFFUSE, t, &texturePath) != aiReturn_SUCCESS)
	//			{
	//				throw std::runtime_error("Could not get diffuse texture info.");
	//			}
	//
	//			auto texture = scene->GetEmbeddedTexture(texturePath.C_Str());
	//
	//			if (not texture)
	//			{
	//				throw std::runtime_error("Could not get diffuse texture.");
	//			}
	//
	//			int twidth, theight, tchannels;
	//			auto bytes = stbi_load_from_memory(
	//				reinterpret_cast<stbi_uc *>(texture->pcData), static_cast<int>(texture->mWidth), &twidth, &theight,
	//				&tchannels, 0);
	//
	//			if (not bytes)
	//			{
	//				throw std::runtime_error("Could not decode texture image.");
	//			}
	//
	//			GLenum internalFormat, format;
	//
	//			switch (tchannels)
	//			{
	//				case 1:
	//				{
	//					internalFormat = GL_R8;
	//					format = GL_RED;
	//					break;
	//				}
	//				case 2:
	//				{
	//					internalFormat = GL_RG8;
	//					format = GL_RG;
	//					break;
	//				}
	//				case 3:
	//				{
	//					internalFormat = GL_RGB8;
	//					format = GL_RGB;
	//					break;
	//				}
	//				case 4:
	//				{
	//					internalFormat = GL_RGBA8;
	//					format = GL_RGBA;
	//					break;
	//				}
	//				default: throw std::runtime_error("Unsupported texture type.");
	//			}
	//
	//			brush.diffuse.push_back(std::make_shared<gles3::Texture>(
	//				GL_TEXTURE_2D, 1, internalFormat, static_cast<GLsizei>(twidth), static_cast<GLsizei>(theight)));
	//			brush.diffuse.back()->subImage2D(
	//				0, 0, 0, static_cast<GLsizei>(twidth), static_cast<GLsizei>(theight), format, GL_UNSIGNED_BYTE,
	//				std::span<const stbi_uc>(bytes, twidth * theight * tchannels));
	//			brush.diffuse.back()->texParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//			brush.diffuse.back()->texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//
	//			stbi_image_free(bytes);
	//		}
	//
	//		m_brushes.push_back(std::move(brush));
	//	}
	//
	//	auto extractIndices = [](aiMesh *mesh) -> std::vector<uint32_t> {
	//		auto indices = std::vector<uint32_t> {};
	//
	//		for (auto i = 0; i < mesh->mNumFaces; ++i)
	//		{
	//			indices.insert(indices.end(), mesh->mFaces[i].mIndices, mesh->mFaces[i].mIndices + 3);
	//		}
	//
	//		return indices;
	//	};
	//
	//	m_meshes.clear();
	//
	//	struct Vertex
	//	{
	//		glm::vec3 position;
	//		glm::vec2 texCoord;
	//	};
	//
	//	const auto vertexAttribs = std::array {
	//		gles3::VertexAttrib {
	//			.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(Vertex), .offset = 0},
	//		gles3::VertexAttrib {
	//			.size = 2, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(Vertex), .offset =
	// sizeof(glm::vec3)},
	//	};
	//
	//	for (auto i = 0; i < scene->mNumMeshes; ++i)
	//	{
	//		const auto mesh = scene->mMeshes[i];
	//
	//		if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
	//		{
	//			Log::debug(fmt::format("Skip mesh '{}'.", mesh->mName.C_Str()));
	//			continue;
	//		}
	//
	//		auto vertices = std::vector<Vertex>(mesh->mNumVertices);
	//
	//		for (auto v = 0; v < mesh->mNumVertices; ++v)
	//		{
	//			auto &vertex = vertices.at(v);
	//
	//			vertex.position = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
	//
	//			if (mesh->mTextureCoords[0])
	//			{
	//				vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
	//			}
	//		}
	//
	//		Log::debug(fmt::format("Loading mesh '{}'.", mesh->mName.C_Str()));
	//
	//		auto vbo = gles3::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices);
	//		auto vao = gles3::VertexArray(vbo, vertexAttribs);
	//
	//		m_meshes.push_back(rendering::Mesh {
	//			.vbo = std::make_shared<gles3::Buffer>(std::move(vbo)),
	//			.ibo = std::make_shared<gles3::Buffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, extractIndices(mesh)),
	//			.vao = std::make_shared<gles3::VertexArray>(std::move(vao)),
	//			.mode = GL_TRIANGLES,
	//			.count = static_cast<GLsizei>(mesh->mNumFaces * 3)});
	//		m_renderList.emplace_back(i, mesh->mMaterialIndex);
	//	}
}

void Game::initLogFile(const std::filesystem::path &path)
{
	auto stream = std::make_shared<std::fstream>(util::openStream(path, std::ios_base::out | std::ios_base::app));

	Log::instance().writer("logfile", [stream](const std::string &message) {
		*stream << message;
		stream->flush();
	});
}

void Game::gles3DebugCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *message, const void *)
{
	Log::debug(message);
}

} // namespace dust::game