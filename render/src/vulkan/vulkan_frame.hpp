#pragma once

#include <memory>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/frame.hpp>

#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

class VulkanFrame final : public Frame
{
public:
	VulkanFrame(
		std::optional<VulkanSwapchainData> swapchain, vk::raii::RenderPass renderPass,
		std::shared_ptr<class VulkanRenderer> renderer);

private:
	[[nodiscard]] vk::Extent2D getFrameImageExtent() const;

	[[nodiscard]] std::vector<vk::raii::ImageView> createImageViews();
	[[nodiscard]] std::vector<vk::raii::Framebuffer> createFrameBuffers();

	std::shared_ptr<class VulkanRenderer> m_renderer;

	std::optional<VulkanSwapchainData> m_swapchain;
	vk::raii::RenderPass m_renderPass;
	std::vector<vk::raii::ImageView> m_imageViews;
	std::vector<vk::raii::Framebuffer> m_frameBuffers;
};

} // namespace dust::render::vulkan
