#pragma once

#include "vulkan_basic_renderer.hpp"

namespace dust::render::vulkan
{

class VulkanSurfaceRenderer final : public VulkanBasicRenderer
{
public:
	VulkanSurfaceRenderer(
		const vk::raii::SurfaceKHR &surface, std::pair<vk::raii::PhysicalDevice, uint32_t> physicalDevice,
		std::shared_ptr<class VulkanBackend> backend);

	void startFrame();
	void endFrame();
};

} // namespace dust::render::vulkan
