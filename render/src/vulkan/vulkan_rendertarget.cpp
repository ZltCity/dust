#include "vulkan_basic_swapchain.hpp"

namespace dust::render::vulkan
{

VulkanRendertarget::VulkanRendertarget(
	vk::Framebuffer framebuffer, vk::Semaphore semaphore, std::shared_ptr<VulkanBasicSwapchain> swapchain)
	: framebuffer {framebuffer}, semaphore {semaphore}, m_swapchain {std::move(swapchain)}
{}

void VulkanRendertarget::present() const
{
	m_swapchain->present(*this);
}

} // namespace dust::render::vulkan
