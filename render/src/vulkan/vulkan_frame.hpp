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
	explicit VulkanFrame(std::shared_ptr<class VulkanRenderer> renderer);

	void clearColor(const std::array<float, 4> &value) final;
	void present() final;

private:
	[[nodiscard]] vk::Extent2D getFrameImageExtent() const;

	[[nodiscard]] vk::Format chooseDepthBufferFormat() const;

	[[nodiscard]] std::pair<vk::raii::Queue, uint32_t> getQueue();
	[[nodiscard]] vk::raii::RenderPass createRenderPass();
	[[nodiscard]] std::vector<vk::raii::ImageView> createSwapchainImageViews();
	[[nodiscard]] vk::raii::Image createDepthBufferImage();
	[[nodiscard]] vk::raii::ImageView createDepthBufferImageView();
	[[nodiscard]] std::vector<vk::raii::Framebuffer> createFrameBuffers();

	std::shared_ptr<class VulkanRenderer> m_renderer;

	std::pair<vk::raii::Queue, uint32_t> m_queue;
	vk::raii::RenderPass m_renderPass;
	vk::raii::Image m_depthBufferImage;
	std::vector<vk::raii::Framebuffer> m_frameBuffers;
};

} // namespace dust::render::vulkan
