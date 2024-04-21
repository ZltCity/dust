#pragma once

#include <string>
#include <utility>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace dust::render::vulkan
{

[[nodiscard]] std::vector<std::string> checkLayersAvailability(
	const std::vector<vk::LayerProperties> &layerProperties, const std::vector<const char *> &layerNames);
[[nodiscard]] std::vector<std::string> checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &extensionProperties, const std::vector<const char *> &extensionNames);

[[nodiscard]] std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> getSuitablePhysicalDevices(
	const vk::raii::Instance &instance, const std::vector<vk::PhysicalDeviceType> &possibleTypes,
	const std::vector<vk::QueueFlagBits> &requiredQueueFlags);
[[nodiscard]] std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> getSuitablePhysicalDevices(
	const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface,
	const std::vector<vk::PhysicalDeviceType> &possibleTypes, const std::vector<vk::QueueFlagBits> &requiredQueueFlags);

[[nodiscard]] std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> getSuitableQueueFamilies(
	const vk::raii::PhysicalDevice &physicalDevice, const std::vector<vk::QueueFlagBits> &requiredQueueFlags);
[[nodiscard]] std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> getSuitableQueueFamilies(
	const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice,
	const std::vector<vk::QueueFlagBits> &requiredQueueFlags);

} // namespace dust::render::vulkan
