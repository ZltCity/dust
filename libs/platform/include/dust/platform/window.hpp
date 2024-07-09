#pragma once

#include <EGL/egl.h>

namespace dust::platform
{

class Window
{
public:
	Window() = default;
	Window(const Window &) = delete;
	virtual ~Window() noexcept = 0;

	Window &operator=(const Window &) = delete;

	[[nodiscard]] virtual EGLNativeWindowType nativeHandle() const = 0;
};

} // namespace dust::platform
