#include <fmt/format.h>

#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>		// Output data structure

#include <glm/gtc/matrix_transform.hpp>

#include "../gles3/rendering_context.hpp"
#include "../logging/log.hpp"
#include "../sdl/sdl.hpp"
#include "../util/blob.hpp"
#include "../util/stream.hpp"
#include "game.hpp"

namespace dust::game
{

using Log = logging::Log;

Game::Game(int argc, char **argv) : m_camera(glm::vec3(0.f, 1.5f, 2.f), glm::vec3(0.f))
{
	initLogFile();

	m_camera.perspective(glm::radians(95.f), 16.f / 9.f, 0.1f, 100.f);
}

int Game::start()
{
	Log::info("Start Dust Game..");

	m_config = loadConfig(configPath);

	auto &sdl = sdl::Lib::instance();
	auto window =
		std::make_shared<sdl::Window>("Dust Game", std::make_tuple(1280, 720), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	auto renderingContext = gles3::RenderingContext(window, m_config.gles3.debugContext);

	renderingContext.makeCurrent();

	if (m_config.gles3.debugContext)
	{
		glDebugMessageCallback(gles3DebugCallback, nullptr);
	}

	auto quit = false;

	//
	loadMap("./assets/maps/test/test.glb");
	//

	while (not quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		m_brushes.back().ubo.back().second->update(0, m_camera.projection(), m_camera.view());

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_brushes.back().set();

		for (const auto &mesh : m_meshes)
		{
			mesh.draw();
		}

		renderingContext.swapBuffers();

		m_camera.roll(0.001f);
	}

	saveConfig(m_config, configPath);

	return 0;
}

void Game::loadMap(const std::filesystem::path &path)
{
	Log::debug(fmt::format("Loading map '{}'.", path.generic_string()));

	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene *scene = importer.ReadFile(
		path.generic_string(),
		//											 aiProcess_CalcTangentSpace |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	// If the import failed, report it
	if (nullptr == scene)
	{
		throw std::runtime_error(fmt::format("Could not import glb file '{}'.", path.generic_string()));
	}

	auto shaders = std::array {
		gles3::Shader(GL_VERTEX_SHADER, util::loadBlob("./assets/shaders/basic.vs")),
		gles3::Shader(GL_FRAGMENT_SHADER, util::loadBlob("./assets/shaders/basic.fs")),
		gles3::Shader(GL_GEOMETRY_SHADER, util::loadBlob("./assets/shaders/wireframe.gs")),
	};

	for (auto &shader : shaders)
	{
		if (not shader.compile())
		{
			throw std::runtime_error(shader.log());
		}
	}

	auto shaderProgram = gles3::ShaderProgram(shaders);

	if (not shaderProgram.link())
	{
		throw std::runtime_error(shaderProgram.log());
	}

	m_brushes = {
		rendering::Brush {
			.shaderProgram = std::make_shared<gles3::ShaderProgram>(std::move(shaderProgram)),
			.ubo = {std::make_pair(std::string("Transform"), std::make_shared<gles3::Buffer>(GL_UNIFORM_BUFFER))},
		},
	};

	m_brushes.back().ubo.back().second->push(GL_STATIC_DRAW, glm::mat4(1.f), glm::mat4(1.f));

	auto extractIndices = [](aiMesh *mesh) -> std::vector<uint32_t> {
		auto indices = std::vector<uint32_t> {};

		for (auto i = 0; i < mesh->mNumFaces; ++i)
		{
			indices.insert(indices.end(), mesh->mFaces[i].mIndices, mesh->mFaces[i].mIndices + 3);
		}

		return indices;
	};

	m_meshes.clear();

	const auto vertexAttribs = std::array {
		gles3::VertexAttrib {
			.size = 3, .type = GL_FLOAT, .normalized = GL_FALSE, .stride = sizeof(aiVector3D), .offset = 0},
	};

	for (auto i = 0; i < scene->mNumMeshes; ++i)
	{
		const auto mesh = scene->mMeshes[i];

		if (mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
		{
			Log::debug(fmt::format("Skip mesh '{}'.", mesh->mName.C_Str()));
			continue;
		}

		Log::debug(fmt::format("Loading mesh '{}'.", mesh->mName.C_Str()));

		auto vbo = gles3::Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, std::span(mesh->mVertices, mesh->mNumVertices));
		auto vao = gles3::VertexArray(vbo, vertexAttribs);

		m_meshes.push_back(rendering::Mesh {
			.vbo = std::make_shared<gles3::Buffer>(std::move(vbo)),
			.ibo = std::make_shared<gles3::Buffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, extractIndices(mesh)),
			.vao = std::make_shared<gles3::VertexArray>(std::move(vao)),
			.mode = GL_TRIANGLES,
			.count = static_cast<GLsizei>(mesh->mNumFaces * 3)});
	}
}

void Game::initLogFile(const std::filesystem::path &path)
{
	auto stream = std::make_shared<std::fstream>(util::openStream(path, std::ios_base::out | std::ios_base::app));

	Log::instance().writer("logfile", [stream](const std::string &message) {
		*stream << message;
		stream->flush();
	});
}

void Game::gles3DebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
	const void *userParam)
{
	Log::debug(message);
}

} // namespace dust::game