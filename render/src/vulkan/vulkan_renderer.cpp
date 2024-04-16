#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanRenderer::VulkanRenderer(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily)
	: m_device {createVulkanDevice(physicalDevice, queueFamily)},
	  m_commandPool {createVulkanCommandPool(m_device, queueFamily)}
{}

vk::raii::Device VulkanRenderer::createVulkanDevice(
	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily)
{
	const auto queuePriorities = std::array {1.f};
	const auto deviceQueueCreateInfo = std::array {vk::DeviceQueueCreateInfo {{}, queueFamily, queuePriorities}};

	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, deviceQueueCreateInfo}};
}

vk::raii::CommandPool VulkanRenderer::createVulkanCommandPool(const vk::raii::Device &device, uint32_t queueFamily)
{
	return vk::raii::CommandPool {device, vk::CommandPoolCreateInfo {{}, queueFamily}};
}

} // namespace dust::render::vulkan
