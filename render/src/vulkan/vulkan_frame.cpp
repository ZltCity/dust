#include "vulkan_frame.hpp"
#include "vulkan_renderer.hpp"

namespace dust::render::vulkan
{

VulkanFrame::VulkanFrame(
	std::optional<std::pair<vk::raii::SwapchainKHR, vk::SurfaceFormatKHR>> swapchain, vk::raii::RenderPass renderPass,
	std::shared_ptr<VulkanRenderer> renderer)
	: m_renderer {std::move(renderer)}, m_swapchain {std::move(swapchain)}, m_renderPass {std::move(renderPass)}
{}

} // namespace dust::render::vulkan
