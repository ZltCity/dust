#pragma once

#include <memory>

#if defined(WITH_SDL)
#include <SDL.h>
#endif

#include <dust/glue/application_info.hpp>

#include "backend.hpp"

namespace dust::render
{

class Renderer
{
public:
	Renderer() = default;
	Renderer(const Renderer &) = delete;
	Renderer(Renderer &&) noexcept = delete;

	virtual ~Renderer() noexcept = 0;

	Renderer &operator=(const Renderer &) = delete;
	Renderer &operator=(Renderer &&) noexcept = delete;
};

[[nodiscard]] std::unique_ptr<Renderer> createRenderer(
	const glue::ApplicationInfo &applicationInfo, Backend backend = Backend::Vulkan);
#if defined(WITH_SDL)
[[nodiscard]] std::unique_ptr<Renderer> createRenderer(
	SDL_Window *window, const glue::ApplicationInfo &applicationInfo, Backend backend = Backend::Vulkan);
#endif

} // namespace dust::render
