#pragma once

#include <memory>

#include <dust/egl/display.hpp>

namespace dust::egl
{

class Surface
{
public:
	Surface();
	Surface(std::shared_ptr<Display> display, EGLConfig config, EGLNativeWindowType window);
	Surface(const Surface &) = delete;
	Surface(Surface &&other) noexcept;
	~Surface() noexcept;

	Surface &operator=(const Surface &) = delete;
	Surface &operator=(Surface &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] EGLSurface handle() const;

	void swapBuffers() const;

	void swap(Surface &other);
	void reset() noexcept;

private:
	std::shared_ptr<Display> m_display;
	EGLSurface m_surface;
};

} // namespace dust::egl