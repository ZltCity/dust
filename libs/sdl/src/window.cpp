#include <stdexcept>

#include <dust/sdl/window.hpp>

namespace dust::sdl
{

Window::Window(const std::string &title, const std::tuple<int32_t, int32_t> &extent, Uint32 flags)
	: m_window(SDL_CreateWindow(title.c_str(), 0, 0, std::get<0>(extent), std::get<1>(extent), flags))
{}

Window::Window(Window &&other) noexcept : m_window {}
{
	swap(other);
}

Window::~Window() noexcept
{
	reset();
}

Window &Window::operator=(Window &&other) noexcept
{
	reset();
	swap(other);

	return *this;
}

Window::operator bool() const
{
	return m_window != nullptr;
}

SDL_Window *Window::handle() const
{
	return m_window;
}

void Window::swap(Window &other) noexcept
{
	std::swap(m_window, other.m_window);
}

void Window::reset() noexcept
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);

		m_window = {};
	}
}

} // namespace dust::sdl