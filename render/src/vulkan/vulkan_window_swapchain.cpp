#include "vulkan_window_swapchain.hpp"

namespace dust::render::vulkan
{

VulkanWindowSwapchain::VulkanWindowSwapchain(vk::raii::SurfaceKHR surface) : m_surface {std::move(surface)}
{}

VulkanRendertarget VulkanWindowSwapchain::acquireNextImage()
{
	return {{}, {}, {}};
}

void VulkanWindowSwapchain::present(const VulkanRendertarget &rendertarget) const
{

}

} // namespace dust::render::vulkan
