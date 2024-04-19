#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "vulkan_basic_swapchain.hpp"

namespace dust::render::vulkan
{

class VulkanWindowSwapchain final : public VulkanBasicSwapchain,
									public std::enable_shared_from_this<VulkanWindowSwapchain>
{
public:
	explicit VulkanWindowSwapchain(vk::raii::SurfaceKHR surface);

	[[nodiscard]] VulkanRendertarget acquireNextImage() final;

	void present(const VulkanRendertarget &rendertarget) const final;

private:
	vk::raii::SurfaceKHR m_surface;
};

} // namespace dust::render::vulkan
