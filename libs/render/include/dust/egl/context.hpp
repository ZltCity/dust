#pragma once

#include <dust/egl/surface.hpp>

namespace dust::egl
{

class Context
{
public:
	Context();
	Context(
		std::shared_ptr<Display> display, std::shared_ptr<Surface> surface, EGLConfig config,
		EGLContext shared = EGL_NO_CONTEXT);
	Context(const Context &) = delete;
	Context(Context &&other) noexcept;
	~Context() noexcept;

	Context &operator=(const Context &) = delete;
	Context &operator=(Context &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] EGLContext handle() const;
	[[nodiscard]] Context share() const;

	void makeCurrent() const;

	void swap(Context &other);
	void reset() noexcept;

private:
	constexpr static auto attributes = std::array {
		EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 2, EGL_NONE,
	};

	std::shared_ptr<Display> m_display;
	std::shared_ptr<Surface> m_surface;
	EGLConfig m_config;
	EGLContext m_context;
};

} // namespace dust::egl