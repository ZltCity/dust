#include "vulkan/vulkan_backend.hpp"

namespace dust::render
{

Backend::~Backend() noexcept
{}

#if defined(WITH_SDL)
std::shared_ptr<Backend> createBackend(
	BackendName backendName, const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
{
	switch (backendName)
	{
		case BackendName::Vulkan: return std::make_shared<vulkan::VulkanBackend>(applicationInfo, window);
		default: return {};
	}
}
#endif

std::vector<BackendName> getAvailableBackends()
{
	return {BackendName::Vulkan};
}

} // namespace dust::render
