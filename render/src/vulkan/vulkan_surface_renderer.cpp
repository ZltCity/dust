#include "vulkan_surface_renderer.hpp"

namespace dust::render::vulkan
{

VulkanSurfaceRenderer::VulkanSurfaceRenderer(
	const vk::raii::SurfaceKHR &surface, vk::raii::Device device, std::vector<SuitableQueueFamily> queueFamilies,
	std::shared_ptr<class VulkanBackend> backend)
	: VulkanBasicRenderer {std::move(device), std::move(queueFamilies), std::move(backend)}
{}

void VulkanSurfaceRenderer::startFrame()
{}

void VulkanSurfaceRenderer::endFrame()
{}

} // namespace dust::render::vulkan
