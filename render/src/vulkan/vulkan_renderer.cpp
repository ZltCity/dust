#include <algorithm>

#include <fmt/format.h>

#include "vulkan_backend.hpp"
#include "vulkan_config.hpp"
#include "vulkan_error.hpp"
#include "vulkan_frame.hpp"
#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanRenderer::VulkanRenderer(vk::raii::PhysicalDevice physicalDevice, std::shared_ptr<VulkanBackend> backend)
	: m_backend {std::move(backend)},
	  m_physicalDevice {std::move(physicalDevice)},
	  m_device {createDevice()},
	  m_commandPools {createCommandPools()},
	  m_swapchain {createSwapchain()}
{}

const vk::raii::PhysicalDevice &VulkanRenderer::getPhysicalDevice() const
{
	return m_physicalDevice;
}

const vk::raii::Device &VulkanRenderer::getDevice() const
{
	return m_device;
}

const std::vector<std::pair<vk::raii::CommandPool, uint32_t>> &VulkanRenderer::getCommandPools() const
{
	return m_commandPools;
}

const std::optional<VulkanSwapchainData> &VulkanRenderer::getSwapchain() const
{
	return m_swapchain;
}

bool VulkanRenderer::hasPresentSupport(uint32_t queueFamilyIndex) const
{
	const auto &surface = m_backend->getSurface();

	return surface.has_value() and m_physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, surface.value());
}

std::shared_ptr<Frame> VulkanRenderer::createFrame()
{
	return std::make_shared<VulkanFrame>(shared_from_this());
}

std::vector<const char *> VulkanRenderer::getRequiredDeviceExtensions() const
{
	auto extensions = std::vector<const char *> {};

	if (m_backend->getSurface().has_value())
	{
		extensions.push_back("VK_KHR_swapchain");
	}

	return extensions;
}

vk::SurfaceFormatKHR VulkanRenderer::chooseSurfaceFormat(const vk::raii::SurfaceKHR &surface) const
{
	const auto availableFormats = m_physicalDevice.getSurfaceFormatsKHR(*surface);
	const auto acceptableFormats = VulkanConfig::getInstance().getAcceptableSurfaceFormats();
	const auto surfaceFormat = std::find_first_of(
		availableFormats.begin(), availableFormats.end(), acceptableFormats.begin(), acceptableFormats.end(),
		[](const vk::SurfaceFormatKHR &available, const vk::Format &acceptable) {
			return available.format == acceptable;
		});

	if (surfaceFormat == availableFormats.end())
	{
		throw std::runtime_error {"Could not find suitable surface format."};
	}

	return *surfaceFormat;
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

vk::raii::Device VulkanRenderer::createDevice()
{
	const auto requiredExtensions = getRequiredDeviceExtensions();

	if (const auto missing =
			checkExtensionsAvailability(m_physicalDevice.enumerateDeviceExtensionProperties(), requiredExtensions);
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following device extensions are not available: {}.", fmt::join(missing, ", "))};
	}

	auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo> {};
	auto queuePriorities = std::vector<std::vector<float>> {};

	for (const auto &[props, index] : getSuitableQueueFamilies(
			 m_physicalDevice, m_backend->getSurface(), VulkanConfig::getInstance().getRequiredQueueFlags()))
	{
		queueCreateInfos.emplace_back(
			vk::DeviceQueueCreateFlags {}, index, queuePriorities.emplace_back(props.queueCount, 1.0f));
	}

	return vk::raii::Device {m_physicalDevice, vk::DeviceCreateInfo {{}, queueCreateInfos, {}, requiredExtensions}};
}

std::vector<std::pair<vk::raii::CommandPool, uint32_t>> VulkanRenderer::createCommandPools()
{
	auto commandPools = std::vector<std::pair<vk::raii::CommandPool, uint32_t>> {};

	for (const auto &[queueFamilyProperties, queueFamilyIndex] : getSuitableQueueFamilies(
			 m_physicalDevice, m_backend->getSurface(), VulkanConfig::getInstance().getRequiredQueueFlags()))
	{
		commandPools.emplace_back(
			vk::raii::CommandPool {
				m_device,
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

VulkanSwapchainData VulkanRenderer::createSwapchain()
{
	const auto &surface = m_backend->getSurface();

	if (not surface.has_value())
	{
		throw NoValidSurface {};
	}

	const auto surfaceFormat = chooseSurfaceFormat(surface.value());
	const auto queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();
	auto queueFamilyIndices = std::vector<uint32_t> {};

	for (const auto &[commandPool, queueFamilyIndex] : m_commandPools)
	{
		if ((queueFamilyProperties.at(queueFamilyIndex).queueFlags & vk::QueueFlagBits::eGraphics) and
			m_physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, *(surface.value())))
		{
			queueFamilyIndices.push_back(queueFamilyIndex);
		}
	}

	const auto surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(*(surface.value()));

	return {
		vk::raii::SwapchainKHR {
			m_device,
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
		surfaceFormat, surfaceCapabilities};
}

} // namespace dust::render::vulkan
