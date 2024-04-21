#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render::vulkan
{

class VulkanRenderer : public Renderer, public std::enable_shared_from_this<VulkanRenderer>
{
public:
	VulkanRenderer(vk::raii::PhysicalDevice physicalDevice, std::shared_ptr<class VulkanBackend> backend);

private:
	[[nodiscard]] static std::vector<const char *> getRequiredDeviceExtensions(
		const std::optional<vk::raii::SurfaceKHR> &surface);

	[[nodiscard]] static vk::raii::Device createDevice(
		const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice);
	[[nodiscard]] static std::vector<std::pair<vk::raii::CommandPool, uint32_t>> createCommandPools(
		const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::Device &device);
	//	[[nodiscard]] std::vector<vk::raii::CommandBuffer> createCommandBuffers(
	//		const vk::raii::Device &device, const vk::raii::CommandPool &commandPool);

	std::shared_ptr<class VulkanBackend> m_backend;

	vk::raii::PhysicalDevice m_physicalDevice;
	vk::raii::Device m_device;
	std::vector<std::pair<vk::raii::CommandPool, uint32_t>> m_commandPools;

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
