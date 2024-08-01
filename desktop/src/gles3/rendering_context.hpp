#pragma once

#include <memory>

#include "../egl/context.hpp"
#include "../sdl/window.hpp"

namespace dust::gles3
{

class RenderingContext
{
public:
	explicit RenderingContext(std::shared_ptr<sdl::Window> window, bool debugContext = false);
	RenderingContext(const RenderingContext &) = delete;

	RenderingContext &operator=(const RenderingContext &) = delete;

	void makeCurrent() const;
	void swapBuffers() const;

private:
	constexpr static auto configAttributes = std::array {
		EGL_CONFORMANT,		EGL_OPENGL_ES3_BIT, EGL_DEPTH_SIZE, 24,		  EGL_STENCIL_SIZE, 8, EGL_RENDERABLE_TYPE,
		EGL_OPENGL_ES3_BIT, EGL_SURFACE_TYPE,	EGL_WINDOW_BIT, EGL_NONE,
	};

	std::shared_ptr<sdl::Window> m_window;
	std::shared_ptr<egl::Display> m_display;
	EGLConfig m_config;
	std::shared_ptr<egl::Surface> m_surface;
	egl::Context m_context;
};

} // namespace dust::gles3
