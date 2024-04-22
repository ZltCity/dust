#include "vulkan_config.hpp"

namespace dust::render::vulkan
{

std::vector<vk::PhysicalDeviceType> VulkanConfig::getAcceptableDeviceTypes() const
{
	return {vk::PhysicalDeviceType::eIntegratedGpu, vk::PhysicalDeviceType::eDiscreteGpu};
}

std::vector<vk::QueueFlagBits> VulkanConfig::getRequiredQueueFlags() const
{
	return {vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer};
}

std::vector<vk::Format> VulkanConfig::getAcceptableSurfaceFormats() const
{
	return {vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8A8Unorm};
}

std::vector<vk::Format> VulkanConfig::getAcceptableDepthBufferFormats() const
{
	return {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint};
}

VulkanConfig &VulkanConfig::getInstance()
{
	static auto config = VulkanConfig {};

	return config;
}

} // namespace dust::render::vulkan
