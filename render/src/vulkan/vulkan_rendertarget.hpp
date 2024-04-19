#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

namespace dust::render::vulkan
{

class VulkanRendertarget
{
public:
	VulkanRendertarget(
		vk::Framebuffer framebuffer, vk::Semaphore semaphore, std::shared_ptr<class VulkanBasicSwapchain> swapchain);

	void present() const;

	const vk::Framebuffer framebuffer;
	const vk::Semaphore semaphore;

private:
	std::shared_ptr<class VulkanBasicSwapchain> m_swapchain;
};

} // namespace dust::render::vulkan
