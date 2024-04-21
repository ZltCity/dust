#include "vulkan_surface_renderer.hpp"

namespace dust::render::vulkan
{

VulkanSurfaceRenderer::VulkanSurfaceRenderer(
	const vk::raii::SurfaceKHR &surface, std::pair<vk::raii::PhysicalDevice, uint32_t> physicalDevice,
	std::shared_ptr<class VulkanBackend> backend)
	: VulkanBasicRenderer {std::move(physicalDevice), std::move(backend)}
{}

void VulkanSurfaceRenderer::startFrame()
{}

void VulkanSurfaceRenderer::endFrame()
{}

} // namespace dust::render::vulkan
