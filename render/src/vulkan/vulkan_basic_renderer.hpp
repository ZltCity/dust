#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render::vulkan
{

class VulkanBasicRenderer : public Renderer, public std::enable_shared_from_this<VulkanBasicRenderer>
{
public:
	VulkanBasicRenderer(
		std::pair<vk::raii::PhysicalDevice, uint32_t> physicalDevice,
		std::shared_ptr<class VulkanBackend> backend);

protected:
	[[nodiscard]] static std::vector<std::pair<vk::raii::CommandPool, uint32_t>> createCommandPools(
		const vk::raii::Device &device, const std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> &queueFamilies);
	//	[[nodiscard]] std::vector<vk::raii::CommandBuffer> createCommandBuffers(
	//		const vk::raii::Device &device, const vk::raii::CommandPool &commandPool);

	std::shared_ptr<class VulkanBackend> m_backend;

	std::pair<vk::raii::PhysicalDevice, uint32_t> m_physicalDevice;
	vk::raii::Device m_device;
	std::vector<CommandPool> m_commandPools;

	//	std::optional<vk::SurfaceFormatKHR> m_surfaceFormat;
	//	std::optional<vk::SurfaceCapabilitiesKHR> m_surfaceCapabilities;
	//	std::optional<vk::raii::SwapchainKHR> m_swapchain;
	//	vk::raii::RenderPass m_renderPass;
	//	std::vector<vk::raii::ImageView> m_imageViews;
	//	std::vector<vk::raii::Framebuffer> m_frameBuffers;
	//	vk::raii::Semaphore m_imageAvailableSemaphore;
	//	vk::raii::Fence m_renderFence;
	//	vk::raii::Queue m_renderQueue;
	//	uint32_t m_frameImageIndex;
};

} // namespace dust::render::vulkan
