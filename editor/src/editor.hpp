#pragma once

#include <filesystem>
#include <map>
#include <set>

#include <GLES3/gl32.h>

#include <glm/glm.hpp>

#include <dust/imgui/imgui.hpp>

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
	struct DirectoryTreeNode
	{
		[[nodiscard]] static DirectoryTreeNode buildDirectoryTree(const std::filesystem::path &terminalPath);

		std::filesystem::path path;
		std::vector<std::filesystem::directory_entry> content;
		std::map<std::filesystem::path, DirectoryTreeNode> children;
	};

	void initLogFile();

	//	UI draw functions.
	void drawMainMenu();
	void drawBrushList();
	void drawBrushEditor();
	void drawDirectoryTree(const DirectoryTreeNode &node);
	void drawOpenFileDialog();

	//	Help functions.
	void addNewBrush();
	void selectBrush(int32_t idx);
	[[nodiscard]] reflections::Brush &selectedBrush();
	void raiseOpenFileDialog(const std::filesystem::path &directory, std::set<std::string> extensions);

	[[nodiscard]] static std::vector<std::filesystem::directory_entry> getDirectoryContent(
		const std::filesystem::path &path);

	static void gles3DebugCallback(
		GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
		const void *userParam);

	static constexpr auto logPath = "./log";
	static constexpr auto configPath = "./config.json";

	//	UI common constants.
	static constexpr auto shaderFileExtensions = std::array {".vs", ".fs"};

	//	UI style constants.
	static constexpr auto defaultFontSize = 18.f;

	static constexpr auto colorGray = IM_COL32(64, 64, 64, 255);

	const glm::vec4 m_clearColor;

	Config m_config;
	bool m_quit = false;

	ImFont *defaultFont;

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
		std::filesystem::path directory;
		std::set<std::string> extensions;
		std::vector<std::filesystem::directory_entry> directoryContent;
		DirectoryTreeNode directoryTree;
		bool show = false;
	} openFileDialog;
};

} // namespace dust::editor