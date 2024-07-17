#include <SDL_syswm.h>

#include <dust/gles3/rendering_context.hpp>

namespace dust::gles3
{

[[nodiscard]] EGLNativeWindowType getNativeWindow(SDL_Window *window);

RenderingContext::RenderingContext(std::shared_ptr<sdl::Window> window, bool debugContext)
	: m_window(std::move(window)),
	  m_display(std::make_shared<egl::Display>(EGL_DEFAULT_DISPLAY)),
	  m_config(m_display->chooseConfig(configAttributes)),
	  m_surface(std::make_shared<egl::Surface>(m_display, m_config, getNativeWindow(m_window->handle()))),
	  m_context(m_display, m_surface, m_config, EGL_NO_CONTEXT, debugContext)
{}

void RenderingContext::makeCurrent() const
{
	m_context.makeCurrent();
}

void RenderingContext::swapBuffers() const
{
	m_surface->swapBuffers();
}

EGLNativeWindowType getNativeWindow(SDL_Window *window)
{
	auto sysInfo = SDL_SysWMinfo {
		.version = SDL_version {.major = SDL_MAJOR_VERSION, .minor = SDL_MINOR_VERSION, .patch = SDL_PATCHLEVEL}};

	if (not SDL_GetWindowWMInfo(window, &sysInfo))
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

} // namespace dust::gles3