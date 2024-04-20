#pragma once

#include <memory>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

#include "vulkan_helper_struct.hpp"

namespace dust::render::vulkan
{

class VulkanBasicRenderer : public Renderer, public std::enable_shared_from_this<VulkanBasicRenderer>
{
public:
	VulkanBasicRenderer(
		vk::raii::Device device, std::vector<SuitableQueueFamily> queueFamilies,
		std::shared_ptr<class VulkanBackend> backend);

protected:
	[[nodiscard]] vk::raii::CommandPool createCommandPool(const vk::raii::Device &device, uint32_t queueFamily);
	[[nodiscard]] std::vector<vk::raii::CommandBuffer> createCommandBuffers(
		const vk::raii::Device &device, const vk::raii::CommandPool &commandPool);

	std::shared_ptr<class VulkanBackend> m_backend;

	vk::raii::Device m_device;
	vk::raii::CommandPool m_commandPool;
	std::vector<vk::raii::CommandBuffer> m_commandBuffers;
	vk::raii::RenderPass m_renderPass;
	std::vector<SuitableQueueFamily> m_queueFamilies;

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
