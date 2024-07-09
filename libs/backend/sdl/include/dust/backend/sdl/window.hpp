#pragma once

#include <string>
#include <tuple>

#include <SDL.h>

#include <dust/platform/window.hpp>

namespace dust::backend::sdl
{

class Window final : public dust::platform::Window
{
public:
	Window(const std::string &title, const std::tuple<int32_t, int32_t> &extent, bool fullscreen);
	~Window() noexcept final;

	EGLNativeWindowType nativeHandle() const final;

private:
	SDL_Window *m_window;
};

} // namespace dust::backend::sdl