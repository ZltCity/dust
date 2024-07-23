#include <fmt/format.h>

#include <dust/gles3/rendering_context.hpp>
#include <dust/imgui/imgui.hpp>
#include <dust/logging/log.hpp>
#include <dust/sdl/sdl.hpp>
#include <dust/sdl/window.hpp>

#include "editor.hpp"

namespace dust::editor
{

Editor::Editor() : m_clearColor(0.65f, 0.65f, 0.7f, 1.0f), m_fileManager(storage::FileManager::create())
{
	initLogFile();
}

void Editor::run()
{
	using dust::logging::Log;

	Log::info("Start Dust Game Editor..");

	m_config = loadConfig(*m_fileManager->file(configPath, storage::StorageType::Local));

	auto &sdl = sdl::Lib::instance();
	auto window = std::make_shared<sdl::Window>(
		"Dust Game Editor", std::make_tuple(1280, 720), SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	auto renderingContext = gles3::RenderingContext(window, m_config.gles3.debugContext);

	renderingContext.makeCurrent();

	auto &imgui = imgui::Lib::instance();

	imgui.config(ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad);
	imgui.initContext(window);

	//	ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	imgui.loadFont("assets/fonts/IBMPlexMono-Regular.ttf", 18.0f);

	if (m_config.gles3.debugContext)
	{
		glDebugMessageCallback(gles3DebugCallback, nullptr);
	}

	while (not m_quit)
	{
		auto event = SDL_Event {};

		while (SDL_PollEvent(&event))
		{
			imgui.processEvent(event);

			if (event.type == SDL_QUIT)
			{
				m_quit = true;
			}
		}

		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		imgui.beginFrame();

		drawMainMenu();

		if (brushList.show)
		{
			drawBrushList();
		}

		if (brushEditor.show)
		{
			drawBrushEditor();
		}

		if (openFileDialog.show)
		{
			drawOpenFileDialog();
		}

		imgui.endFrame();
		imgui.present();

		renderingContext.swapBuffers();
	}

	editor::saveConfig(m_config, *m_fileManager->file(configPath, storage::StorageType::Local));
}

void Editor::drawMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				m_quit = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Brushes", nullptr, &brushList.show)) {}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::drawBrushList()
{
	using dust::logging::Log;

	if (ImGui::Begin("Brushes", &brushList.show))
	{
		auto brushIdx = int32_t {};

		for (const auto &brush : brushList.brushes)
		{
			auto selected = (brushIdx == brushList.selectedBrush);

			if (ImGui::Selectable(brush.name.c_str(), &selected))
			{
				if (selected)
				{
					selectBrush(brushIdx);
				}
			}

			++brushIdx;
		}

		if (ImGui::Button("Add new"))
		{
			addNewBrush();
		}
	}

	ImGui::End();
}

void Editor::drawBrushEditor()
{
	if (ImGui::Begin(brushList.brushes.at(brushList.selectedBrush).name.c_str(), &brushEditor.show))
	{
		if (ImGui::CollapsingHeader("Shaders"))
		{
			for (const auto &shader : selectedBrush().shaders)
			{
				if (ImGui::Selectable(shader.sourcePath.generic_string().c_str())) {}
			}

			if (ImGui::Button("Add new"))
			{
				openFileDialog.show = true;
			}
		}
	}

	ImGui::End();
}

void Editor::drawOpenFileDialog()
{
	ImGui::OpenPopup("Open file");

	if (ImGui::BeginPopupModal("Open file", &openFileDialog.show))
	{
		ImGui::Text("Hello dsjfhds fhjs hfj dshfj hds");
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			openFileDialog.show = false;
		}
		ImGui::EndPopup();
	}
}

void Editor::addNewBrush()
{
	brushList.brushes.push_back(reflections::Brush {.name = fmt::format("Brush #{}", brushList.brushes.size())});
	brushList.selectedBrush = static_cast<int32_t>(brushList.brushes.size() - 1);
	brushEditor.show = true;
}

void Editor::selectBrush(int32_t idx)
{
	using dust::logging::Log;

	brushList.selectedBrush = idx;
	brushEditor.show = true;

	Log::debug(fmt::format("Select '{}' brush", brushList.brushes.at(idx).name));
}

reflections::Brush &Editor::selectedBrush()
{
	return brushList.brushes.at(brushList.selectedBrush);
}

void Editor::initLogFile()
{
	using dust::logging::Log;

	const auto sharedStream = std::shared_ptr<std::ostream>(
		m_fileManager->file(logPath, storage::StorageType::Local)->stream(storage::StreamFlags::Append));

	Log::instance().writer("logfile", [stream = sharedStream](const std::string &message) {
		*stream << message;
		stream->flush();
	});
}

void Editor::gles3DebugCallback(
	GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
	const void *userParam)
{
	using dust::logging::Log;

	Log::debug(message);
}

} // namespace dust::editor