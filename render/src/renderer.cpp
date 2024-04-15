#include "vulkan/vulkan_renderer.hpp"

namespace dust::render
{

Renderer::~Renderer() noexcept
{}

std::unique_ptr<Renderer> createRenderer(
	const glue::ApplicationInfo &applicationInfo, Backend backend, std::initializer_list<Hint> hints)
{
	return {};
}

#if defined(WITH_SDL)
std::unique_ptr<Renderer> createRenderer(
	const glue::ApplicationInfo &applicationInfo, SDL_Window *window, Backend backend,
	std::initializer_list<Hint> hints)
{
	switch (backend)
	{
		case Backend::Vulkan: return std::make_unique<VulkanRenderer>(applicationInfo, window, hints);
		default: return {};
	}
}
#endif

} // namespace dust::render
