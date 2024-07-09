#include <stdexcept>

#include <dust/egl/display.hpp>

#include "error.hpp"

namespace dust::egl
{

Display::Display() : m_display(EGL_NO_DISPLAY)
{}

Display::Display(EGLNativeDisplayType nativeDisplay) : m_display(eglGetDisplay(nativeDisplay))
{
	if (not eglInitialize(m_display, nullptr, nullptr))
	{
		throw Error("Could not initialize EGL display.");
	}
}

Display::Display(Display &&other) noexcept : m_display(EGL_NO_DISPLAY)
{
	swap(other);
}

Display::~Display() noexcept
{
	reset();
}

Display &Display::operator=(Display &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Display::operator bool() const
{
	return m_display != EGL_NO_DISPLAY;
}

EGLConfig Display::chooseConfig(std::span<const EGLint> attrs) const
{
	if (not operator bool())
	{
		throw std::logic_error("Invalid EGL display.");
	}

	auto configs = std::array<::EGLConfig, 16>();
	EGLint count = {};

	if (not eglChooseConfig(m_display, attrs.data(), configs.data(), static_cast<EGLint>(configs.size()), &count))
	{
		throw Error("Failed to choose EGL config.");
	}

	return configs.front();
}

EGLDisplay Display::handle() const
{
	return m_display;
}

void Display::swap(Display &other) noexcept
{
	std::swap(m_display, other.m_display);
}

void Display::reset() noexcept
{
	if (m_display != EGL_NO_DISPLAY)
	{
		eglTerminate(m_display);
	}
}

} // namespace dust::egl