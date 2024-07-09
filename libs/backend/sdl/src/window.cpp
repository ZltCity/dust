#include <stdexcept>

#include <SDL_syswm.h>

#include <dust/backend/sdl/window.hpp>

namespace dust::backend::sdl
{

Window::Window(const std::string &title, const std::tuple<int32_t, int32_t> &extent, bool fullscreen)
	: m_window(SDL_CreateWindow(
		  title.c_str(), 0, 0, std::get<0>(extent), std::get<1>(extent),
		  SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)))
{}

Window::~Window() noexcept
{
	SDL_DestroyWindow(m_window);
}

EGLNativeWindowType Window::nativeHandle() const
{
	auto sysInfo = SDL_SysWMinfo {
		.version = SDL_version {.major = SDL_MAJOR_VERSION, .minor = SDL_MINOR_VERSION, .patch = SDL_PATCHLEVEL}};

	if (not SDL_GetWindowWMInfo(m_window, &sysInfo))
	{
		throw std::runtime_error(SDL_GetError());
	}

#if defined(__linux__)
#if defined(SDL_VIDEO_DRIVER_X11)
	return reinterpret_cast<EGLNativeWindowType>(sysInfo.info.x11.window);
#elif defined(SDL_VIDEO_DRIVER_WAYLAND)
	return reinterpret_cast<EGLNativeWindowType>(sysInfo.info.wl.egl_window);
#endif
#elif (defined(_WIN32) || defined(_WIN64)) && defined(SDL_VIDEO_DRIVER_WINDOWS)
	return reinterpret_cast<EGLNativeWindowType>(sysInfo.info.win.window);
#else
	static_assert(false, "Unsupported platform");
#endif
}

} // namespace dust::backend::sdl