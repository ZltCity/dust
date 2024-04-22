#pragma once

#include <memory>
#include <vector>

#if defined(WITH_SDL)
#include <SDL.h>
#endif

#include <dust/glue/application_info.hpp>

#include "device.hpp"
#include "hint.hpp"
#include "renderer.hpp"

namespace dust::render
{

enum class BackendName
{
	Vulkan = 0
};

class Backend
{
public:
	Backend() = default;
	Backend(const Backend &) = delete;
	virtual ~Backend() noexcept = 0;

	Backend &operator=(const Backend &) = delete;

	[[nodiscard]] virtual std::shared_ptr<Renderer> createRenderer(const std::vector<Hint> &hints) = 0;

	[[nodiscard]] virtual std::vector<Device> getSuitableDevices() const = 0;
};

#if defined(WITH_SDL)
[[nodiscard]] std::shared_ptr<Backend> createBackend(
	BackendName backendName, const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
#endif

[[nodiscard]] std::vector<BackendName> getAvailableBackends();

} // namespace dust::render
