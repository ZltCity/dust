#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

class VulkanRenderer : public Renderer, public std::enable_shared_from_this<VulkanRenderer>
{
public:
	VulkanRenderer(vk::raii::PhysicalDevice physicalDevice, std::shared_ptr<class VulkanBackend> backend);

	const vk::raii::Device &getDevice() const;

	std::shared_ptr<Frame> createFrame() final;

private:
	[[nodiscard]] std::vector<const char *> getRequiredDeviceExtensions() const;

	[[nodiscard]] vk::SurfaceFormatKHR chooseSurfaceFormat(const vk::raii::SurfaceKHR &surface) const;
	[[nodiscard]] vk::Format chooseDepthBufferFormat() const;

	[[nodiscard]] vk::raii::Device createDevice();
	[[nodiscard]] std::vector<std::pair<vk::raii::CommandPool, uint32_t>> createCommandPools();
	[[nodiscard]] VulkanSwapchainData createSwapchain();
	[[nodiscard]] vk::raii::RenderPass createRenderPass(vk::Format colorAttachmentFormat);

	std::shared_ptr<class VulkanBackend> m_backend;

	vk::raii::PhysicalDevice m_physicalDevice;
	vk::raii::Device m_device;
	std::vector<std::pair<vk::raii::CommandPool, uint32_t>> m_commandPools;
};

} // namespace dust::render::vulkan
