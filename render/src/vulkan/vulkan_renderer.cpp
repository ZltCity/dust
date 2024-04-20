#include <algorithm>

#include <fmt/format.h>

#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

//[[nodiscard]] vk::raii::Device createVulkanDevice(
//	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount,
//	std::initializer_list<const char *> extensions = {});
//[[nodiscard]] vk::raii::CommandPool createVulkanCommandPool(const vk::raii::Device &device, uint32_t queueFamily);
//[[nodiscard]] std::vector<vk::raii::CommandBuffer> createVulkanCommandBuffers(
//	const vk::raii::Device &device, const vk::raii::CommandPool &commandPool);
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

VulkanRenderer::VulkanRenderer(vk::raii::Device device, std::vector<SuitableQueueFamily> queueFamilies)
	: m_device {std::move(device)}, m_queueFamilies {std::move(queueFamilies)}
//	: m_device {createVulkanDevice(physicalDevice, queueFamily, queueCount, {"VK_KHR_swapchain"})},
//	  m_commandPool {createVulkanCommandPool(m_device, queueFamily)},
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

 void VulkanRenderer::startFrame()
{
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
//			*m_renderPass, *m_frameBuffers[m_frameImageIndex], vk::Rect2D {{}, m_surfaceCapabilities->currentExtent}},
//		vk::SubpassContents::eInline);
 }

 void VulkanRenderer::endFrame()
{
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
 }
//
// vk::raii::Device createVulkanDevice(
//	const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily, uint32_t queueCount,
//	std::initializer_list<const char *> extensions)
//{
//	if (const auto missing =
//			checkExtensionsAvailability(physicalDevice.enumerateDeviceExtensionProperties(), extensions);
//		not missing.empty())
//	{
//		throw std::runtime_error {
//			fmt::format("Following device extensions are not available: {}.", fmt::join(missing, ", "))};
//	}
//
//	const auto queuePriorities = std::vector<float>(queueCount, 1.f);
//	const auto deviceQueueCreateInfo = std::array {vk::DeviceQueueCreateInfo {{}, queueFamily, queuePriorities}};
//
//	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, deviceQueueCreateInfo, {}, extensions}};
// }
//
// vk::raii::CommandPool createVulkanCommandPool(const vk::raii::Device &device, uint32_t queueFamily)
//{
//	return vk::raii::CommandPool {
//		device, vk::CommandPoolCreateInfo {vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueFamily}};
// }
//
// std::vector<vk::raii::CommandBuffer> createVulkanCommandBuffers(
//	const vk::raii::Device &device, const vk::raii::CommandPool &commandPool)
//{
//	return device.allocateCommandBuffers(
//		vk::CommandBufferAllocateInfo {*commandPool, vk::CommandBufferLevel::ePrimary, 1});
// }
//
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
