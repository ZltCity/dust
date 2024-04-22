#include "vulkan_frame.hpp"
#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanFrame::VulkanFrame(
	std::optional<VulkanSwapchainData> swapchain, vk::raii::RenderPass renderPass,
	std::shared_ptr<VulkanRenderer> renderer)
	: m_renderer {std::move(renderer)},
	  m_swapchain {std::move(swapchain)},
	  m_renderPass {std::move(renderPass)},
	  m_imageViews {createImageViews()},
	  m_frameBuffers {createFrameBuffers()}
{}

vk::Extent2D VulkanFrame::getFrameImageExtent() const
{
	if (m_swapchain.has_value())
	{
		return std::get<2>(m_swapchain.value()).currentExtent;
	}
}

std::vector<vk::raii::ImageView> VulkanFrame::createImageViews()
{
	auto imageViews = std::vector<vk::raii::ImageView> {};

	if (m_swapchain.has_value())
	{
		for (const auto &image : std::get<0>(m_swapchain.value()).getImages())
		{
			imageViews.emplace_back(
				m_renderer->getDevice(), vk::ImageViewCreateInfo {
											 {},
											 image,
											 vk::ImageViewType::e2D,
											 std::get<1>(m_swapchain.value()).format,
											 {},
											 vk::ImageSubresourceRange {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});
		}
	}

	return imageViews;
}

std::vector<vk::raii::Framebuffer> VulkanFrame::createFrameBuffers()
{
	const auto frameImageExtent = getFrameImageExtent();
	auto frameBuffers = std::vector<vk::raii::Framebuffer> {};

	for (const auto &imageView : m_imageViews)
	{
		const auto attachments = std::array {*imageView};

		frameBuffers.emplace_back(
			m_renderer->getDevice(), vk::FramebufferCreateInfo {
										 {},
										 *m_renderPass,
										 attachments,
										 frameImageExtent.width,
										 frameImageExtent.height,
										 1,
									 });
	}

	return frameBuffers;
}

} // namespace dust::render::vulkan
