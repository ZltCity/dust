#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

#include <GLES3/gl32.h>

#include <dust/gles3/rendering_context.hpp>
#include <dust/imgui/imgui.hpp>
#include <dust/logging/log.hpp>
#include <dust/sdl/sdl.hpp>
#include <dust/storage/file_manager.hpp>

#include "config.hpp"

constexpr auto configPath = "./config.json";

void gles3DebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
	const void *userParam)
{
	using namespace dust::logging;

	Log::debug(message);
}

int main(int argc, const char **argv)
{
	using namespace dust;
	using dust::logging::Log;

	Log::instance().writer(
		"logfile", [logfile = std::make_shared<std::ofstream>("./log", std::ios_base::app)](
					   const std::string &message) { *logfile << message; });
	Log::info("Start Dust Game Editor..");

	auto &sdl = sdl::Lib::instance();
	auto &imgui = imgui::Lib::instance();

	auto fileManager = storage::FileManager::create();
	auto config = editor::loadConfig(*fileManager->file(configPath, storage::StorageType::Local));

	auto window = std::make_shared<sdl::Window>(
		"Dust Game Editor", std::make_tuple(1280, 720), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	auto renderingContext = dust::gles3::RenderingContext(window, config.gles3.debugContext);
	auto quit = false;

	renderingContext.makeCurrent();

	imgui.config(ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad);
	imgui.initContext(window);

	//	ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	imgui.loadFont("assets/fonts/IBMPlexMono-Regular.ttf", 18.0f);

	auto clearColor = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);

	if (config.gles3.debugContext)
	{
		glDebugMessageCallback(gles3DebugCallback, nullptr);
	}

	while (not quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event))
		{
			imgui.processEvent(event);

			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		imgui.beginFrame();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open")) {}

				ImGui::Separator();

				if (ImGui::MenuItem("Quit"))
				{
					quit = true;
				}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		imgui.endFrame();
		imgui.present();

		renderingContext.swapBuffers();
	}

	editor::saveConfig(config, *fileManager->file(configPath, storage::StorageType::Local));

	return 0;
}