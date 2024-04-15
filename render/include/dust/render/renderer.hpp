#pragma once

#include <initializer_list>
#include <memory>
#include <utility>

#if defined(WITH_SDL)
#include <SDL.h>
#endif

#include <dust/glue/application_info.hpp>

#include "backend.hpp"
#include "hint.hpp"

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
	const glue::ApplicationInfo &applicationInfo, Backend backend = Backend::Vulkan,
	std::initializer_list<Hint> hints = {});

#if defined(WITH_SDL)
[[nodiscard]] std::unique_ptr<Renderer> createRenderer(
	const glue::ApplicationInfo &applicationInfo, SDL_Window *window, Backend backend = Backend::Vulkan,
	std::initializer_list<Hint> hints = {});
#endif

} // namespace dust::render
