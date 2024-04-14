#include "vulkan/vulkan_renderer.hpp"

namespace dust::render
{

Renderer::~Renderer() noexcept
{}

std::unique_ptr<Renderer> createRenderer(const glue::ApplicationInfo &applicationInfo, Backend backend)
{
	return {};
}

#if defined(WITH_SDL)
std::unique_ptr<Renderer> createRenderer(
	const glue::ApplicationInfo &applicationInfo, SDL_Window *window, Backend backend)
{
	switch (backend)
	{
		case Backend::Vulkan: return std::make_unique<VulkanRenderer>(applicationInfo, window);
		default: return {};
	}
}
#endif

} // namespace dust::render
