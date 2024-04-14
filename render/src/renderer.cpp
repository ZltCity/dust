#include <dust/render/renderer.hpp>

#include "vulkan/vulkan_renderer.hpp"

namespace dust::render
{

Renderer::~Renderer() noexcept
{}

#if defined(WITH_SDL)
std::unique_ptr<Renderer> createRenderer(
	SDL_Window *window, const glue::ApplicationInfo &applicationInfo, Backend backend)
{
	switch (backend)
	{
		case Backend::Vulkan: return std::make_unique<VulkanRenderer>(applicationInfo);
		default: return {};
	}
}
#endif

} // namespace dust::render
