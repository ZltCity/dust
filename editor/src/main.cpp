#include <fstream>
#include <iostream>

#include <fmt/format.h>
#include <glm/glm.hpp>

#include <GLES3/gl32.h>

#include <dust/gles3/buffer.hpp>
#include <dust/gles3/rendering_context.hpp>
#include <dust/gles3/shader_program.hpp>
#include <dust/imgui/imgui.hpp>
#include <dust/logging/log.hpp>
#include <dust/scene/scene.hpp>
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

	struct
	{
		bool showSkyboxProperties = false;
	} uiState;

	struct
	{
		struct
		{
			std::filesystem::path vertexShader, fragmentShader, leftImage, rightImage, nearImage, farImage, bottomImage,
				topImage;
		} skyboxProperties;

		bool drawSkybox = true;
	} sceneState;

	std::unique_ptr<scene::Scene> scene;

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
				//				if (ImGui::MenuItem("Open")) {}
				//
				//				ImGui::Separator();

				if (ImGui::MenuItem("Quit"))
				{
					quit = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("New"))
				{
					scene = std::make_unique<scene::Scene>();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Save")) {}
				if (ImGui::MenuItem("Load")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Show skybox properties", nullptr, &uiState.showSkyboxProperties)) {}

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (uiState.showSkyboxProperties)
		{
			if (ImGui::Begin("Skybox Properties", &uiState.showSkyboxProperties))
			{
				if (ImGui::Checkbox("Draw skybox", &sceneState.drawSkybox)) {}

				ImGui::Text("%s", sceneState.skyboxProperties.leftImage.c_str());
			}

			ImGui::End();
		}

		imgui.endFrame();
		imgui.present();

		if (scene)
		{
			scene->present(sceneState.drawSkybox);
		}

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