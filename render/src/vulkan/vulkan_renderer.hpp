#pragma once

#include <initializer_list>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render::vulkan
{

class VulkanRenderer final : public Renderer
{
public:
	VulkanRenderer(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount);
	VulkanRenderer(
		const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily,
		uint32_t queueCount);

private:
	[[nodiscard]] static vk::raii::Device createVulkanDevice(
		const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount,
		std::initializer_list<const char *> extensions = {});
	[[nodiscard]] static vk::raii::CommandPool createVulkanCommandPool(
		const vk::raii::Device &device, uint32_t queueFamily);
	[[nodiscard]] static vk::raii::SwapchainKHR createVulkanSwapchain(
		const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice,
		const vk::raii::Device &device, uint32_t queueFamily);

	[[nodiscard]] static vk::SurfaceFormatKHR chooseSurfaceFormat(
		const vk::raii::SurfaceKHR &surface, const vk::raii::PhysicalDevice &physicalDevice,
		std::initializer_list<vk::Format> requiredFormats);

	vk::raii::Device m_device;
	vk::raii::CommandPool m_commandPool;
	std::optional<vk::raii::SwapchainKHR> m_swapchain;
};

} // namespace dust::render::vulkan
