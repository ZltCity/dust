#pragma once

#include <GLES3/gl32.h>

#include <glm/glm.hpp>

#include <dust/storage/file_manager.hpp>

#include "config.hpp"
#include "reflections/brush.hpp"

namespace dust::editor
{

class Editor
{
public:
	Editor();
	Editor(const Editor &) = delete;

	Editor &operator=(const Editor &) = delete;

	void run();

private:
	void initLogFile();

	//	UI draw functions.
	void drawMainMenu();
	void drawBrushList();
	void drawBrushEditor();
	void drawOpenFileDialog();

	//	Help functions.
	void addNewBrush();
	void selectBrush(int32_t idx);
	[[nodiscard]] reflections::Brush &selectedBrush();

	static void gles3DebugCallback(
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
		const void *userParam);

	static constexpr auto logPath = "./log";
	static constexpr auto configPath = "./config.json";

	const glm::vec4 m_clearColor;

	std::unique_ptr<storage::FileManager> m_fileManager;
	Config m_config;
	bool m_quit = false;

	struct
	{
		std::vector<reflections::Brush> brushes;
		int32_t selectedBrush = {};
		bool show = true;
	} brushList;

	struct
	{
		bool show = false;
	} brushEditor;

	struct
	{
		bool show = false;
	} openFileDialog;
};

} // namespace dust::editor