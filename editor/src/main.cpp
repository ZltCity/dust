#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

#include <GLES3/gl32.h>

#include <dust/gles3/buffer.hpp>
#include <dust/gles3/rendering_context.hpp>
#include <dust/gles3/shader_program.hpp>
#include <dust/imgui/imgui.hpp>
#include <dust/logging/log.hpp>
#include <dust/sdl/sdl.hpp>
#include <dust/storage/file_manager.hpp>

#include "config.hpp"

constexpr auto configPath = "./config.json";
constexpr auto logPath = "./log";

void initLogFile(dust::storage::FileManager &fileManager);
void gles3DebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
	const void *userParam);

int main(int argc, const char **argv)
{
	using namespace dust;
	using dust::logging::Log;

	auto fileManager = storage::FileManager::create();

	initLogFile(*fileManager);

	Log::info("Start Dust Game Editor..");

	auto config = editor::loadConfig(*fileManager->file(configPath, storage::StorageType::Local));
	auto &sdl = sdl::Lib::instance();
	auto window = std::make_shared<sdl::Window>(
		"Dust Game Editor", std::make_tuple(1280, 720), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	auto renderingContext = dust::gles3::RenderingContext(window, config.gles3.debugContext);
	auto quit = false;

	renderingContext.makeCurrent();

	auto &imgui = imgui::Lib::instance();

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void initLogFile(dust::storage::FileManager &fileManager)
{
	using namespace dust;
	using dust::logging::Log;

	const auto sharedStream = std::shared_ptr<std::ostream>(
		fileManager.file(logPath, storage::StorageType::Local)->stream(storage::StreamFlags::Append));

	Log::instance().writer("logfile", [stream = sharedStream](const std::string &message) {
		*stream << message;
		stream->flush();
	});
}

void gles3DebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
	const void *userParam)
{
	using namespace dust::logging;

	Log::debug(message);
}