#include <algorithm>

#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanRenderer::VulkanRenderer(
	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount)
	: m_device {createVulkanDevice(physicalDevice, queueFamily, queueCount)},
	  m_commandPool {createVulkanCommandPool(m_device, queueFamily)}
{}

VulkanRenderer::VulkanRenderer(
	const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily,
	uint32_t queueCount)
	: m_device {createVulkanDevice(physicalDevice, queueFamily, queueCount, {"VK_KHR_swapchain"})},
	  m_commandPool {createVulkanCommandPool(m_device, queueFamily)},
	  m_swapchain {createVulkanSwapchain(surface, physicalDevice, m_device, queueFamily)}
{}

vk::raii::Device VulkanRenderer::createVulkanDevice(
	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount,
	std::initializer_list<const char *> extensions)
{
	const auto queuePriorities = std::vector<float>(queueCount, 1.f);
	const auto deviceQueueCreateInfo = std::array {vk::DeviceQueueCreateInfo {{}, queueFamily, queuePriorities}};

	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, deviceQueueCreateInfo, {}, extensions}};
}

vk::raii::CommandPool VulkanRenderer::createVulkanCommandPool(const vk::raii::Device &device, uint32_t queueFamily)
{
	return vk::raii::CommandPool {device, vk::CommandPoolCreateInfo {{}, queueFamily}};
}

vk::raii::SwapchainKHR VulkanRenderer::createVulkanSwapchain(
	const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::Device &device,
	uint32_t queueFamily)
{
	const auto surfaceFormat = chooseSurfaceFormat(surface, physicalDevice, {vk::Format::eR8G8B8A8Unorm});
	const auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
	const auto queueFamilyIndices = std::array {queueFamily};
	auto swapchain = vk::raii::SwapchainKHR {
		device, vk::SwapchainCreateInfoKHR {
					{},
					*surface,
					surfaceCapabilities.minImageCount,
					surfaceFormat.format,
					surfaceFormat.colorSpace,
					surfaceCapabilities.currentExtent,
					1,
					vk::ImageUsageFlagBits::eColorAttachment,
					vk::SharingMode::eExclusive,
					queueFamilyIndices,
					vk::SurfaceTransformFlagBitsKHR::eIdentity,
					vk::CompositeAlphaFlagBitsKHR::eOpaque,
					vk::PresentModeKHR::eFifo,
					true}};

	return swapchain;
}

vk::SurfaceFormatKHR VulkanRenderer::chooseSurfaceFormat(
	const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice,
	std::initializer_list<vk::Format> requiredFormats)
{
	const auto availableSurfaceFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
	const auto surfaceFormat = std::find_first_of(
		availableSurfaceFormats.begin(), availableSurfaceFormats.end(), requiredFormats.begin(), requiredFormats.end(),
		[](const vk::SurfaceFormatKHR &available, const vk::Format &required) { return available.format == required; });

	if (surfaceFormat == availableSurfaceFormats.end())
	{
		throw std::runtime_error {"Could not find suitable surface format."};
	}

	return *surfaceFormat;
}

} // namespace dust::render::vulkan
