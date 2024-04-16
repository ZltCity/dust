#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanRenderer::VulkanRenderer(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily)
	: m_device {createVulkanDevice(physicalDevice, queueFamily)}
{}

vk::raii::Device VulkanRenderer::createVulkanDevice(
	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily)
{
	const auto queuePriorities = std::array {1.f};
	const auto deviceQueueCreateInfo = std::array {vk::DeviceQueueCreateInfo {{}, queueFamily, queuePriorities}};

	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, deviceQueueCreateInfo}};
}

} // namespace dust::render::vulkan
