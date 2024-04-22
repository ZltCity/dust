#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

namespace dust::render::vulkan
{

class VulkanConfig
{
public:
	VulkanConfig(const VulkanConfig &) = delete;
	VulkanConfig &operator=(const VulkanConfig &) = delete;

	[[nodiscard]] std::vector<vk::PhysicalDeviceType> getAcceptableDeviceTypes() const;
	[[nodiscard]] std::vector<vk::QueueFlagBits> getRequiredQueueFlags() const;
	[[nodiscard]] std::vector<vk::Format> getAcceptableSurfaceFormats() const;
	[[nodiscard]] std::vector<vk::Format> getAcceptableDepthBufferFormats() const;

	[[nodiscard]] static VulkanConfig &getInstance();

private:
	VulkanConfig() = default;
};

} // namespace dust::render::vulkan
