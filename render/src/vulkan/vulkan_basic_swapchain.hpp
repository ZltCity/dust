#pragma once

#include <utility>

#include "vulkan_rendertarget.hpp"

namespace dust::render::vulkan
{

class VulkanBasicSwapchain
{
public:
	VulkanBasicSwapchain() = default;
	VulkanBasicSwapchain(const VulkanBasicSwapchain &) = delete;
	VulkanBasicSwapchain(VulkanBasicSwapchain &&) noexcept = delete;

	virtual ~VulkanBasicSwapchain() = 0;

	VulkanBasicSwapchain &operator=(const VulkanBasicSwapchain &) = delete;
	VulkanBasicSwapchain &operator=(VulkanBasicSwapchain &&) noexcept = delete;

	[[nodiscard]] virtual VulkanRendertarget acquireNextImage() = 0;

	virtual void present(const VulkanRendertarget &rendertarget) const = 0;
};

} // namespace dust::render::vulkan
