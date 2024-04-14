#include <algorithm>
#include <stdexcept>

#include <fmt/format.h>

#if defined(WITH_SDL)
#include <SDL_vulkan.h>
#endif

#include "vulkan_renderer.hpp"

namespace dust::render
{

VulkanRenderer::VulkanRenderer(const glue::ApplicationInfo &applicationInfo)
	: m_context {}, m_instance {createVulkanInstance(m_context, applicationInfo)}
{}

#if defined(WITH_SDL)
VulkanRenderer::VulkanRenderer(const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
	: m_context {}, m_instance {createVulkanInstance(m_context, applicationInfo, window)}
{}

vk::raii::Instance VulkanRenderer::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
{
	const auto vkAppInfo = vk::ApplicationInfo {
		.pApplicationName = applicationInfo.applicationName.c_str(),
		.applicationVersion = applicationInfo.applicationVersion,
		.pEngineName = DUST_ENGINE_NAME,
		.engineVersion = DUST_ENGINE_VERSION,
		.apiVersion = VK_API_VERSION_1_3,
	};
	const auto layers = std::vector<const char *>
	{
#if !defined(NDEBUG)
		"VK_LAYER_KHRONOS_validation"
#endif
	};
	const auto extensions = getRequiredVulkanExtensions(window);

	checkLayersAvailability(context.enumerateInstanceLayerProperties(), layers);
	checkExtensionsAvailability(context.enumerateInstanceExtensionProperties(), extensions);

	return vk::raii::Instance {
		context, vk::InstanceCreateInfo {
					 .pApplicationInfo = &vkAppInfo,
					 .enabledLayerCount = static_cast<uint32_t>(layers.size()),
					 .ppEnabledLayerNames = layers.data(),
					 .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
					 .ppEnabledExtensionNames = extensions.data()}};
}

std::vector<const char *> VulkanRenderer::getRequiredVulkanExtensions(SDL_Window *window)
{
	unsigned int count = 0;

	if (not SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr))
	{
		throw std::runtime_error {"'SDL_Vulkan_GetInstanceExtensions' failed on get extensions count."};
	}

	auto extensions = std::vector<const char *>(count, nullptr);

	if (not SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data()))
	{
		throw std::runtime_error {"'SDL_Vulkan_GetInstanceExtensions' failed on get extensions names."};
	}

	return extensions;
}

#endif

vk::raii::Instance VulkanRenderer::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo)
{
	throw std::logic_error {"Vulkan offscreen rendering is not implemented."};
}

void VulkanRenderer::checkLayersAvailability(
	const std::vector<vk::LayerProperties> &availableLayers, const std::vector<const char *> &requiredLayers)
{
	for (auto layerName : requiredLayers)
	{
		if (std::find_if(availableLayers.begin(), availableLayers.end(), [layerName](auto &&prop) {
				return std::string_view {layerName} == prop.layerName;
			}) == availableLayers.end())
		{
			throw std::runtime_error {fmt::format("Layer '{}' is unavailable.", layerName)};
		}
	}
}

void VulkanRenderer::checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &availableExtensions,
	const std::vector<const char *> &requiredExtensions)
{
	for (auto extensionName : requiredExtensions)
	{
		if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [extensionName](auto &&prop) {
				return std::string_view {extensionName} == prop.extensionName;
			}) == availableExtensions.end())
		{
			throw std::runtime_error {fmt::format("Extension '{}' is unavailable.", extensionName)};
		}
	}
}

} // namespace dust::render
