#include <algorithm>

#include <fmt/format.h>

#include "vulkan_backend.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

//[[nodiscard]] vk::raii::SwapchainKHR createVulkanSwapchain(
//	const vk::raii::Device &device, const vk::raii::SurfaceKHR &surface, const vk::SurfaceFormatKHR &surfaceFormat,
//	const vk::SurfaceCapabilitiesKHR &surfaceCapabilities, uint32_t queueFamily);
//[[nodiscard]] vk::raii::RenderPass createVulkanRenderPass(const vk::raii::Device &device, vk::Format
// frameImageFormat);
//[[nodiscard]] std::vector<vk::raii::ImageView> createVulkanImageViews(
//	const vk::raii::Device &device, const vk::SurfaceFormatKHR &surfaceFormat, const vk::raii::SwapchainKHR &swapchain);
//[[nodiscard]] std::vector<vk::raii::Framebuffer> createVulkanFrameBuffers(
//	const vk::raii::Device &device, const vk::Extent2D &imageExtent, const vk::raii::RenderPass &renderPass,
//	const std::vector<vk::raii::ImageView> &imageViews);
//
//[[nodiscard]] vk::SurfaceFormatKHR chooseSurfaceFormat(
//	const std::vector<vk::SurfaceFormatKHR> &availableFormats, const std::vector<vk::Format> &requiredFormats);

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

// vk::raii::SwapchainKHR createVulkanSwapchain(
//	const vk::raii::Device &device, const vk::raii::SurfaceKHR &surface, const vk::SurfaceFormatKHR &surfaceFormat,
//	const vk::SurfaceCapabilitiesKHR &surfaceCapabilities, uint32_t queueFamily)
//{
//	const auto queueFamilyIndices = std::array {queueFamily};
//	auto swapchain = vk::raii::SwapchainKHR {
//		device, vk::SwapchainCreateInfoKHR {
//					{},
//					*surface,
//					surfaceCapabilities.minImageCount,
//					surfaceFormat.format,
//					surfaceFormat.colorSpace,
//					surfaceCapabilities.currentExtent,
//					1,
//					vk::ImageUsageFlagBits::eColorAttachment,
//					vk::SharingMode::eExclusive,
//					queueFamilyIndices,
//					vk::SurfaceTransformFlagBitsKHR::eIdentity,
//					vk::CompositeAlphaFlagBitsKHR::eOpaque,
//					vk::PresentModeKHR::eFifo,
//					true}};
//
//	return swapchain;
// }
//
// vk::raii::RenderPass createVulkanRenderPass(const vk::raii::Device &device, vk::Format frameImageFormat)
//{
//	const auto attachments = std::array {vk::AttachmentDescription {
//		{},
//		frameImageFormat,
//		vk::SampleCountFlagBits::e1,
//		vk::AttachmentLoadOp::eDontCare,
//		vk::AttachmentStoreOp::eStore,
//		vk::AttachmentLoadOp::eDontCare,
//		vk::AttachmentStoreOp::eStore,
//		vk::ImageLayout::eUndefined,
//		vk::ImageLayout::ePresentSrcKHR,
//	}};
//	const auto attachmentRefs = std::array {vk::AttachmentReference {0, vk::ImageLayout::eColorAttachmentOptimal}};
//	const auto subpasses = std::array {vk::SubpassDescription {
//		{},
//		vk::PipelineBindPoint::eGraphics,
//		{},
//		attachmentRefs,
//	}};
//
//	return vk::raii::RenderPass {device, vk::RenderPassCreateInfo {{}, attachments, subpasses}};
// }
//
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
//
// vk::SurfaceFormatKHR chooseSurfaceFormat(
//	const std::vector<vk::SurfaceFormatKHR> &availableFormats, const std::vector<vk::Format> &requiredFormats)
//{
//	const auto surfaceFormat = std::find_first_of(
//		availableFormats.begin(), availableFormats.end(), requiredFormats.begin(), requiredFormats.end(),
//		[](const vk::SurfaceFormatKHR &available, const vk::Format &required) { return available.format == required; });
//
//	if (surfaceFormat == availableFormats.end())
//	{
//		throw std::runtime_error {"Could not find suitable surface format."};
//	}
//
//	return *surfaceFormat;
// }

} // namespace dust::render::vulkan
