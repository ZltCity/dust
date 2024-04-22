#include "vulkan_frame.hpp"
#include "vulkan_config.hpp"
#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanFrame::VulkanFrame(std::shared_ptr<VulkanRenderer> renderer)
	: m_renderer {std::move(renderer)},
	  m_queue {getQueue()},
	  m_renderPass {createRenderPass()},
	  m_depthBufferImage {createDepthBufferImage()},
	  m_frameBuffers {createFrameBuffers()}
{}

void VulkanFrame::clearColor(const std::array<float, 4> &value)
{}

void VulkanFrame::present()
{}

vk::Extent2D VulkanFrame::getFrameImageExtent() const
{
	const auto &swapchain = m_renderer->getSwapchain();

	if (swapchain.has_value())
	{
		return std::get<2>(swapchain.value()).currentExtent;
	}
}

vk::Format VulkanFrame::chooseDepthBufferFormat() const
{
	const auto acceptableFormats = VulkanConfig::getInstance().getAcceptableDepthBufferFormats();
	const auto depthBufferFormat =
		std::find_if(acceptableFormats.begin(), acceptableFormats.end(), [this](vk::Format format) {
			const auto formatProperties = m_renderer->getPhysicalDevice().getFormatProperties(format);

			return formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment;
		});

	if (depthBufferFormat == acceptableFormats.end())
	{
		throw std::runtime_error {"Could not find suitable depth buffer format."};
	}

	return *depthBufferFormat;
}

std::pair<vk::raii::Queue, uint32_t> VulkanFrame::getQueue()
{
	const auto queueFamilyProperties = m_renderer->getPhysicalDevice().getQueueFamilyProperties();

	for (const auto &[commandPool, queueFamilyIndex] : m_renderer->getCommandPools())
	{
		if ((queueFamilyProperties.at(queueFamilyIndex).queueFlags & vk::QueueFlagBits::eGraphics) and
			m_renderer->hasPresentSupport(queueFamilyIndex))
		{
			return {vk::raii::Queue {m_renderer->getDevice(), queueFamilyIndex, 0}, queueFamilyIndex};
		}
	}

	throw std::runtime_error {"Could not find suitable queue."};
}

vk::raii::RenderPass VulkanFrame::createRenderPass()
{
	const auto &swapchain = m_renderer->getSwapchain();
	const auto colorAttachmentFormat =
		swapchain.has_value() ? std::get<1>(swapchain.value()).format : vk::Format::eB8G8R8A8Unorm;
	const auto attachments = std::array {
		vk::AttachmentDescription {
			{},
			colorAttachmentFormat,
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR,
		},
		vk::AttachmentDescription {
			{},
			chooseDepthBufferFormat(),
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal}};
	const auto colorAttachmentRefs = std::array {vk::AttachmentReference {0, vk::ImageLayout::eColorAttachmentOptimal}};
	const auto depthAttachmentRef = vk::AttachmentReference {1, vk::ImageLayout::eDepthStencilAttachmentOptimal};
	const auto subpasses = std::array {vk::SubpassDescription {
		{}, vk::PipelineBindPoint::eGraphics, {}, colorAttachmentRefs, {}, &depthAttachmentRef}};
	const auto subpassDependencies = std::array {vk::SubpassDependency {
		VK_SUBPASS_EXTERNAL,
		0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
		{},
		vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite}};

	return vk::raii::RenderPass {
		m_renderer->getDevice(), vk::RenderPassCreateInfo {{}, attachments, subpasses, subpassDependencies}};
}

std::vector<vk::raii::ImageView> VulkanFrame::createSwapchainImageViews()
{
	const auto &swapchain = m_renderer->getSwapchain();
	auto imageViews = std::vector<vk::raii::ImageView> {};

	if (swapchain.has_value())
	{
		for (const auto &image : std::get<0>(swapchain.value()).getImages())
		{
			imageViews.emplace_back(
				m_renderer->getDevice(), vk::ImageViewCreateInfo {
											 {},
											 image,
											 vk::ImageViewType::e2D,
											 std::get<1>(swapchain.value()).format,
											 {},
											 vk::ImageSubresourceRange {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});
		}
	}

	return imageViews;
}

vk::raii::Image VulkanFrame::createDepthBufferImage()
{
	const auto frameImageExtent = getFrameImageExtent();
	const auto queueFamilies = std::array {std::get<1>(m_queue)};

	return vk::raii::Image {
		m_renderer->getDevice(), vk::ImageCreateInfo {
									 {},
									 vk::ImageType::e2D,
									 chooseDepthBufferFormat(),
									 vk::Extent3D {frameImageExtent.width, frameImageExtent.height, 1},
									 1,
									 1,
									 vk::SampleCountFlagBits::e1,
									 vk::ImageTiling::eOptimal,
									 vk::ImageUsageFlagBits::eDepthStencilAttachment,
									 vk::SharingMode::eExclusive,
									 queueFamilies}};
}

vk::raii::ImageView VulkanFrame::createDepthBufferImageView()
{
	return vk::raii::ImageView {
		m_renderer->getDevice(), vk::ImageViewCreateInfo {
									 {},
									 *m_depthBufferImage,
									 vk::ImageViewType::e2D,
									 chooseDepthBufferFormat(),
									 {},
									 vk::ImageSubresourceRange {vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1}}};
}

std::vector<vk::raii::Framebuffer> VulkanFrame::createFrameBuffers()
{
	const auto frameImageExtent = getFrameImageExtent();
	auto frameBuffers = std::vector<vk::raii::Framebuffer> {};

	for (const auto &imageView : createSwapchainImageViews())
	{
		const auto depthBufferImageView = createDepthBufferImageView();
		const auto attachments = std::array {*imageView, *depthBufferImageView};

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
