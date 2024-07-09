#include <dust/gles3/rendering_context.hpp>

namespace dust::gles3
{

RenderingContext::RenderingContext(std::shared_ptr<platform::Window> window)
	: m_window(std::move(window)),
	  m_display(std::make_shared<egl::Display>(EGL_DEFAULT_DISPLAY)),
	  m_config(m_display->chooseConfig(configAttributes)),
	  m_surface(std::make_shared<egl::Surface>(m_display, m_config, m_window->nativeHandle())),
	  m_context(m_display, m_surface, m_config)
{}

void RenderingContext::makeCurrent() const
{
	m_context.makeCurrent();
}

void RenderingContext::preset() const
{
	m_surface->swapBuffers();
}

} // namespace dust::gles3