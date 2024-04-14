#pragma once

#include <string>
#include <vector>

#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render
{

class VulkanRenderer final : public Renderer
{
public:
	explicit VulkanRenderer(const glue::ApplicationInfo &applicationInfo);

#if defined(WITH_SDL)
	VulkanRenderer(const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
#endif

private:
	[[nodiscard]] static vk::raii::Instance createVulkanInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo);

#if defined(WITH_SDL)
	[[nodiscard]] static vk::raii::Instance createVulkanInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
	[[nodiscard]] static std::vector<const char *> getRequiredVulkanExtensions(SDL_Window *window);
#endif

	static void checkLayersAvailability(
		const std::vector<vk::LayerProperties> &availableLayers, const std::vector<const char *> &requiredLayers);
	static void checkExtensionsAvailability(
		const std::vector<vk::ExtensionProperties> &availableExtensions,
		const std::vector<const char *> &requiredExtensions);

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
};

} // namespace dust::render
