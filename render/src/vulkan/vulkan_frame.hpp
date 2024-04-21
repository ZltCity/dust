#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/frame.hpp>

namespace dust::render::vulkan
{

class VulkanFrame final : public Frame
{
public:
	VulkanFrame(
		std::optional<std::pair<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR>> swapchain,
		vk::raii::RenderPass renderPass, std::shared_ptr<class VulkanRenderer> renderer);

private:
	std::shared_ptr<class VulkanRenderer> m_renderer;

	std::optional<std::pair<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR>> m_swapchain;
	vk::raii::RenderPass m_renderPass;
	std::vector<vk::raii::ImageView> m_imageViews;
	std::vector<vk::raii::Framebuffer> m_frameBuffers;
};

} // namespace dust::render::vulkan
