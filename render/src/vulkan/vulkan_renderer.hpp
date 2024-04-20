#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

#include "vulkan_helper_struct.hpp"

namespace dust::render::vulkan
{

class VulkanRenderer final : public Renderer, public std::enable_shared_from_this<VulkanRenderer>
{
public:
	VulkanRenderer(vk::raii::Device device, std::vector<SuitableQueueFamily> queueFamilies);

	void startFrame() final;
	void endFrame() final;

private:
	vk::raii::Device m_device;
	std::vector<SuitableQueueFamily> m_queueFamilies;
	//	vk::raii::CommandPool m_commandPool;
	//	std::vector<vk::raii::CommandBuffer> m_commandBuffers;
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
