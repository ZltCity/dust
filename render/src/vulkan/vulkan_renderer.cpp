#include <algorithm>

#include <fmt/format.h>

#include "vulkan_backend.hpp"
#include "vulkan_error.hpp"
#include "vulkan_frame.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

VulkanRenderer::VulkanRenderer(vk::raii::PhysicalDevice physicalDevice, std::shared_ptr<VulkanBackend> backend)
	: m_backend {std::move(backend)},
	  m_physicalDevice {std::move(physicalDevice)},
	  m_device {createDevice(*m_backend, m_physicalDevice)},
	  m_commandPools {createCommandPools(*m_backend, m_physicalDevice, m_device)}
//	  m_commandBuffers {createVulkanCommandBuffers(m_device, m_commandPool)},
//	  m_surfaceFormat {chooseSurfaceFormat(
//		  physicalDevice.getSurfaceFormatsKHR(*surface), {vk::Format::eR8G8B8A8Unorm, vk::Format::eB8G8R8A8Unorm})},
//	  m_surfaceCapabilities {physicalDevice.getSurfaceCapabilitiesKHR(*surface)},
//	  m_swapchain {createVulkanSwapchain(m_device, surface, *m_surfaceFormat, *m_surfaceCapabilities, queueFamily)},
//	  m_renderPass {createVulkanRenderPass(m_device, m_surfaceFormat->format)},
//	  m_imageViews {createVulkanImageViews(m_device, *m_surfaceFormat, *m_swapchain)},
//	  m_frameBuffers {
//		  createVulkanFrameBuffers(m_device, m_surfaceCapabilities->currentExtent, m_renderPass, m_imageViews)},
//	  m_imageAvailableSemaphore {m_device, vk::SemaphoreCreateInfo {}},
//	  m_renderFence {m_device, vk::FenceCreateInfo {}},
//	  m_renderQueue {m_device, queueFamily, 0}
{}

std::shared_ptr<Frame> VulkanRenderer::createFrame()
{
	auto swapchain = std::optional<std::pair<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR>> {};

	if (m_backend->getSurface().has_value())
	{
		swapchain = std::make_optional(createSwapchain(*m_backend, m_physicalDevice, m_device, m_commandPools));
	}

	const auto colorAttachmentFormat =
		swapchain.has_value() ? std::get<1>(swapchain.value()).format : vk::Format::eB8G8R8A8Unorm;

	return std::make_shared<VulkanFrame>(
		std::move(swapchain), createRenderPass(*m_backend, m_physicalDevice, m_device, colorAttachmentFormat),
		shared_from_this());
}

const vk::raii::Device &VulkanRenderer::getDevice() const
{
	return m_device;
}

std::vector<const char *> VulkanRenderer::getRequiredDeviceExtensions(
	const std::optional<vk::raii::SurfaceKHR> &surface)
{
	auto extensions = std::vector<const char *> {};

	if (surface.has_value())
	{
		extensions.push_back("VK_KHR_swapchain");
	}

	return extensions;
}

vk::SurfaceFormatKHR VulkanRenderer::chooseSurfaceFormat(
	const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::SurfaceKHR &surface,
	const std::vector<vk::Format> &possibleFormats)
{
	const auto availableFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
	const auto surfaceFormat = std::find_first_of(
		availableFormats.begin(), availableFormats.end(), possibleFormats.begin(), possibleFormats.end(),
		[](const vk::SurfaceFormatKHR &available, const vk::Format &possible) { return available.format == possible; });

	if (surfaceFormat == availableFormats.end())
	{
		throw std::runtime_error {"Could not find suitable surface format."};
	}

	return *surfaceFormat;
}

vk::Format VulkanRenderer::chooseDepthBufferFormat(
	const vk::raii::PhysicalDevice &physicalDevice, const std::vector<vk::Format> &possibleFormats)
{
	const auto depthBufferFormat =
		std::find_if(possibleFormats.begin(), possibleFormats.end(), [&physicalDevice](vk::Format format) {
			const auto formatProperties = physicalDevice.getFormatProperties(format);

			return formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment;
		});

	if (depthBufferFormat == possibleFormats.end())
	{
		throw std::runtime_error {"Could not find suitable depth buffer format."};
	}

	return *depthBufferFormat;
}

// void VulkanBasicRenderer::startFrame()
//{
//	const auto image = m_swapchain->acquireNextImage(UINT64_MAX, *m_imageAvailableSemaphore, {});
//
//	if (image.first != vk::Result::eSuccess)
//	{
//		return;
//	}
//
//	m_frameImageIndex = image.second;
//
//	auto &cmd = m_commandBuffers.front();
//
//	cmd.reset({});
//	cmd.begin(vk::CommandBufferBeginInfo {});
//	cmd.beginRenderPass(
//		vk::RenderPassBeginInfo {
//			*m_renderPass, *m_frameBuffers[m_frameImageIndex], vk::Rect2D {{},
//  m_surfaceCapabilities->currentExtent}}, 		vk::SubpassContents::eInline);
//}

// void VulkanBasicRenderer::endFrame()
//{
//	auto &cmd = m_commandBuffers.front();
//
//	cmd.endRenderPass();
//	cmd.end();
//
//	m_device.resetFences({*m_renderFence});vk::raii::Semaphore
//
//	const auto waitSemaphores = std::array {*m_imageAvailableSemaphore};
//	const auto pipelineStageFlags =
//		std::array<vk::PipelineStageFlags, 1> {vk::PipelineStageFlagBits::eColorAttachmentOutput};
//	const auto commandBuffers = std::array {*cmd};
//	const auto submitInfo = std::array {vk::SubmitInfo {waitSemaphores, pipelineStageFlags, commandBuffers}};
//
//	m_renderQueue.submit(submitInfo, *m_renderFence);
//
//	while (m_device.waitForFences({*m_renderFence}, true, 0) == vk::Result::eTimeout)
//		;
//
//	const auto swapchains = std::array {*(m_swapchain.value())};
//	const auto images = std::array {m_frameImageIndex};
//
//	m_renderQueue.presentKHR(vk::PresentInfoKHR {{}, swapchains, images});
//
//	m_device.waitIdle();
//}

vk::raii::Device VulkanRenderer::createDevice(
	const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice)
{
	const auto requiredExtensions = getRequiredDeviceExtensions(backend.getSurface());

	if (const auto missing =
			checkExtensionsAvailability(physicalDevice.enumerateDeviceExtensionProperties(), requiredExtensions);
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following device extensions are not available: {}.", fmt::join(missing, ", "))};
	}

	auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo> {};
	auto queuePriorities = std::vector<std::vector<float>> {};

	for (const auto &[props, index] :
		 getSuitableQueueFamilies(physicalDevice, backend.getSurface(), backend.requiredQueueFlags))
	{
		queueCreateInfos.emplace_back(
			vk::DeviceQueueCreateFlags {}, index, queuePriorities.emplace_back(props.queueCount, 1.0f));
	}

	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, queueCreateInfos, {}, requiredExtensions}};
}

std::vector<std::pair<vk::raii::CommandPool, uint32_t>> VulkanRenderer::createCommandPools(
	const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::Device &device)
{
	auto commandPools = std::vector<std::pair<vk::raii::CommandPool, uint32_t>> {};

	for (const auto &[queueFamilyProperties, queueFamilyIndex] :
		 getSuitableQueueFamilies(physicalDevice, backend.getSurface(), backend.requiredQueueFlags))
	{
		commandPools.emplace_back(
			vk::raii::CommandPool {
				device,
				vk::CommandPoolCreateInfo {vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamilyIndex}},
			queueFamilyIndex);
	}

	return commandPools;
}

// std::vector<vk::raii::CommandBuffer> VulkanBasicRenderer::createCommandBuffers(
//	const vk::raii::Device &device, const vk::raii::CommandPool &commandPool)
//{
//	return device.allocateCommandBuffers(
//		vk::CommandBufferAllocateInfo {*commandPool, vk::CommandBufferLevel::ePrimary, 1});
// }

std::pair<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR> VulkanRenderer::createSwapchain(
	const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::Device &device,
	const std::vector<std::pair<vk::raii::CommandPool, uint32_t>> &commandPools)
{
	const auto &surface = backend.getSurface();

	if (not surface.has_value())
	{
		throw NoValidSurface {};
	}

	const auto surfaceFormat = chooseSurfaceFormat(physicalDevice, surface.value(), backend.possibleSurfaceFormats);
	const auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	auto queueFamilyIndices = std::vector<uint32_t> {};

	for (const auto &[commandPool, queueFamilyIndex] : commandPools)
	{
		if ((queueFamilyProperties.at(queueFamilyIndex).queueFlags & vk::QueueFlagBits::eGraphics) and
			physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, *(surface.value())))
		{
			queueFamilyIndices.push_back(queueFamilyIndex);
		}
	}

	const auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*(surface.value()));

	return {
		vk::raii::SwapchainKHR {
			device,
			vk::SwapchainCreateInfoKHR {
				{},
				*(surface.value()),
				surfaceCapabilities.minImageCount,
				surfaceFormat.format,
				surfaceFormat.colorSpace,
				surfaceCapabilities.currentExtent,
				1,
				vk::ImageUsageFlagBits::eColorAttachment,
				vk::SharingMode::eExclusive,
				queueFamilyIndices,
				vk::SurfaceTransformFlagBitsKHR::eIdentity,
				vk::CompositeAlphaFlagBitsKHR::eOpaque,
				vk::PresentModeKHR::eFifo,
				true}},
		surfaceFormat};
}

vk::raii::RenderPass VulkanRenderer::createRenderPass(
	const VulkanBackend &backend, const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::Device &device,
	vk::Format colorAttachmentFormat)
{
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
			chooseDepthBufferFormat(physicalDevice, backend.possibleDepthBufferFormats),
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

	return vk::raii::RenderPass {device, vk::RenderPassCreateInfo {{}, attachments, subpasses, subpassDependencies}};
}

// std::vector<vk::raii::ImageView> createVulkanImageViews(
//	const vk::raii::Device &device, const vk::SurfaceFormatKHR &surfaceFormat, const vk::raii::SwapchainKHR &swapchain)
//{
//	auto imageViews = std::vector<vk::raii::ImageView> {};
//
//	for (const auto &image : swapchain.getImages())
//	{
//		imageViews.emplace_back(
//			device, vk::ImageViewCreateInfo {
//						{},
//						image,
//						vk::ImageViewType::e2D,
//						surfaceFormat.format,
//						{},
//						vk::ImageSubresourceRange {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}});
//	}
//
//	return imageViews;
// }
//
// std::vector<vk::raii::Framebuffer> createVulkanFrameBuffers(
//	const vk::raii::Device &device, const vk::Extent2D &imageExtent, const vk::raii::RenderPass &renderPass,
//	const std::vector<vk::raii::ImageView> &imageViews)
//{
//	auto frameBuffers = std::vector<vk::raii::Framebuffer> {};
//
//	for (const auto &imageView : imageViews)
//	{
//		const auto attachments = std::array {*imageView};
//
//		frameBuffers.emplace_back(
//			device, vk::FramebufferCreateInfo {
//						{},
//						*renderPass,
//						attachments,
//						imageExtent.width,
//						imageExtent.height,
//						1,
//					});
//	}
//
//	return frameBuffers;
// }

} // namespace dust::render::vulkan
