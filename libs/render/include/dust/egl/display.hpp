#pragma once

#include <span>

#include <EGL/egl.h>

namespace dust::egl
{

class Display
{
public:
	Display();
	explicit Display(EGLNativeDisplayType nativeDisplay);
	Display(const Display &) = delete;
	Display(Display &&other) noexcept;
	~Display() noexcept;

	Display &operator=(const Display &) = delete;
	Display &operator=(Display &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] EGLConfig chooseConfig(std::span<const EGLint> attrs) const;
	[[nodiscard]] EGLDisplay handle() const;

	void swap(Display &other) noexcept;
	void reset() noexcept;

private:
	EGLDisplay m_display;
};

} // namespace dust::egl