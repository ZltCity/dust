#pragma once

#include <memory>

#if defined(WITH_SDL)
#include <SDL.h>
#endif

#include "backend.hpp"

namespace dust::render
{

class Renderer
{
public:
	virtual ~Renderer() noexcept = 0;
};

[[nodiscard]] std::unique_ptr<Renderer> createRenderer(Backend backend = Backend::Vulkan);
#if defined(WITH_SDL)
[[nodiscard]] std::unique_ptr<Renderer> createRenderer(SDL_Window *window, Backend backend = Backend::Vulkan);
#endif

} // namespace dust::render
