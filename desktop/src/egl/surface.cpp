#include "surface.hpp"

#include "error.hpp"

namespace dust::egl
{

Surface::Surface() : m_surface(EGL_NO_SURFACE)
{}

Surface::Surface(std::shared_ptr<Display> display, EGLConfig config, EGLNativeWindowType window)
	: m_display(std::move(display)), m_surface(eglCreateWindowSurface(m_display->handle(), config, window, nullptr))
{
	if (m_surface == EGL_NO_SURFACE)
	{
		throw Error("Could not create EGL surface.");
	}
}

Surface::Surface(Surface &&other) noexcept : m_surface(EGL_NO_SURFACE)
{
	swap(other);
}

Surface::~Surface() noexcept
{
	reset();
}

Surface &Surface::operator=(Surface &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Surface::operator bool() const
{
	return m_surface != EGL_NO_SURFACE;
}

EGLSurface Surface::handle() const
{
	return m_surface;
}

void Surface::swapBuffers() const
{
	if (not operator bool())
	{
		throw std::logic_error("Invalid EGL surface.");
	}

	if (not eglSwapBuffers(m_display->handle(), m_surface))
	{
		throw Error("Could not swap surface buffers.");
	}
}

void Surface::swap(Surface &other)
{
	std::swap(m_display, other.m_display);
	std::swap(m_surface, other.m_surface);
}

void Surface::reset() noexcept
{
	if (m_surface != EGL_NO_SURFACE)
	{
		eglDestroySurface(m_display->handle(), m_surface);

		m_surface = EGL_NO_SURFACE;
		m_display = {};
	}
}

} // namespace dust::egl