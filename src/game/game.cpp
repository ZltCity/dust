#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL_mouse.h>

#include "../logging/log.hpp"
#include "../renderer/renderer.hpp"
#include "../sdl/sdl.hpp"
#include "../sdl/window.hpp"
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
		SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | (m_config.window.fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	auto renderer = renderer::Renderer::create(m_config.renderer.debug);

	auto quit = false;
	auto relativeMouse = false;

	loadMap({});

	//	auto renderer = scene::Renderer(*m_scene);

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
					const auto yawAngle = static_cast<float>(event.motion.xrel) * m_config.camera.sensitivity *
										  (m_config.camera.inverseX ? 1.f : -1.f);
					const auto pitchAngle = static_cast<float>(event.motion.yrel) * m_config.camera.sensitivity *
											(m_config.camera.inverseY ? 1.f : -1.f);

					camera.yaw(yawAngle);
					camera.pitch(pitchAngle);

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
						default: break;
					}
					break;
				}
			}
		}

		const auto [right, up, forward] = camera.axes();

		if (moveForward and m_config.camera.flyMode.enabled)
		{
			camera.move(forward * m_config.camera.flyMode.velocity);
		}
		if (moveBackward)
		{
			camera.move(forward * m_config.camera.flyMode.velocity * -1.f);
		}
		if (moveLeft)
		{
			camera.move(right * m_config.camera.flyMode.velocity * -1.f);
		}
		if (moveRight)
		{
			camera.move(right * m_config.camera.flyMode.velocity);
		}

		const auto [wndWidth, wndHeight] = window->size();

		//		renderer.view(
		//			scene::Camera::perspective(
		//				glm::radians(m_config.camera.fov), static_cast<float>(wndWidth) / static_cast<float>(wndHeight),
		// 0.1f, 				10000.f), 			camera.view()); 		renderer.draw(m_scene->present());
	}

	saveConfig(m_config, configPath);

	return 0;
}

void Game::loadMap(const std::filesystem::path &path)
{
	Log::debug(fmt::format("Loading map '{}'.", path.generic_string()));

	m_scene = std::make_unique<scene::Scene>("./assets", "graveyard");
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