#pragma once

#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace dust::render::vulkan
{

using VulkanSwapchainData = std::tuple<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR, vk::SurfaceCapabilitiesKHR>;

[[nodiscard]] std::vector<std::string> checkLayersAvailability(
	const std::vector<vk::LayerProperties> &layerProperties, const std::vector<const char *> &requiredLayers);
[[nodiscard]] std::vector<std::string> checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &extensionProperties,
	const std::vector<const char *> &requiredExtensions);

[[nodiscard]] std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> getSuitablePhysicalDevices(
	const vk::raii::Instance &instance, const std::optional<vk::raii::SurfaceKHR> &surface,
	const std::vector<vk::PhysicalDeviceType> &acceptableTypes,
	const std::vector<vk::QueueFlagBits> &requiredQueueFlags);
[[nodiscard]] std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> getSuitableQueueFamilies(
	const vk::raii::PhysicalDevice &physicalDevice, const std::optional<vk::raii::SurfaceKHR> &surface,
	const std::vector<vk::QueueFlagBits> &requiredQueueFlags);

} // namespace dust::render::vulkan
