#pragma once

#include <string>
#include <tuple>

#include <SDL.h>

namespace dust::sdl
{

class Window
{
public:
	Window(const std::string &title, const std::tuple<int32_t, int32_t> &extent, Uint32 flags = {});
	Window(const Window &) = delete;
	Window(Window &&other) noexcept;
	~Window() noexcept;

	Window &operator=(const Window &) = delete;
	Window &operator=(Window &&other) noexcept;

	[[nodiscard]] operator bool() const;

	[[nodiscard]] SDL_Window *handle() const;

	[[nodiscard]] std::tuple<int32_t, int32_t> size() const;

	void swap(Window &other) noexcept;
	void reset() noexcept;

private:
	SDL_Window *m_window;
};

} // namespace dust::sdl