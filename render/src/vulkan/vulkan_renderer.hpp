#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render::vulkan
{

class VulkanRenderer final : public Renderer
{
public:
	VulkanRenderer(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily);

private:
	[[nodiscard]] static vk::raii::Device createVulkanDevice(
		const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily);

	vk::raii::Device m_device;
};

} // namespace dust::render::vulkan
