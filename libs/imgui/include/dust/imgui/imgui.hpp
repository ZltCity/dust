#pragma once

#include <filesystem>
#include <memory>

#include <imgui.h>

#include <dust/sdl/window.hpp>

namespace dust::imgui
{

class Lib
{
public:
	Lib(const Lib &) = delete;
	~Lib() noexcept;

	Lib &operator=(const Lib &) = delete;

	void initContext(std::shared_ptr<sdl::Window> window);
	void config(ImGuiConfigFlags flags);

	ImFont *loadFont(const std::filesystem::path &path, float sizePixels);

	void beginFrame();
	void endFrame();

	void present();

	void processEvent(const SDL_Event &event);

	[[nodiscard]] float framerate() const;

	[[nodiscard]] static Lib &instance();

private:
	Lib();

	constexpr static auto GLSLVersion = "#version 300 es";

	std::shared_ptr<sdl::Window> m_window;
	ImGuiContext *m_context;
};

} // namespace dust::imgui