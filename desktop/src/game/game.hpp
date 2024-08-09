#pragma once

#include <vector>

#include <GLES3/gl32.h>

#include "../scene/camera.hpp"
#include "../scene/material.hpp"
#include "../scene/mesh.hpp"
#include "../scene/scene.hpp"
#include "config.hpp"

namespace dust::game
{

class Game
{
public:
	Game(int argc, char **argv);
	Game(const Game &) = delete;

	Game &operator=(const Game &) = delete;

	[[nodiscard]] int start();

private:
	static constexpr auto configPath = "./config.json";

	void loadMap(const std::filesystem::path &path);

	static void initLogFile(const std::filesystem::path &path = "./log");
	static void gles3DebugCallback(
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
		const void *userParam);

	Config m_config;

	std::unique_ptr<scene::Scene> m_scene;

//	Camera m_camera;
//	std::shared_ptr<gles3::Buffer> m_transformUBO;
//	std::vector<rendering::Brush> m_brushes;
//	std::vector<rendering::Mesh> m_meshes;
//	std::vector<std::pair<int, int>> m_renderList;
};

} // namespace dust::game