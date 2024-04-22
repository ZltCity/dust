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

	[[nodiscard]] const vk::raii::PhysicalDevice &getPhysicalDevice() const;
	[[nodiscard]] const vk::raii::Device &getDevice() const;
	[[nodiscard]] const std::vector<std::pair<vk::raii::CommandPool, uint32_t>> &getCommandPools() const;
	[[nodiscard]] const std::optional<VulkanSwapchainData> &getSwapchain() const;

	[[nodiscard]] bool hasPresentSupport(uint32_t queueFamilyIndex) const;

	[[nodiscard]] std::shared_ptr<Frame> createFrame() final;

private:
	[[nodiscard]] std::vector<const char *> getRequiredDeviceExtensions() const;

	[[nodiscard]] vk::SurfaceFormatKHR chooseSurfaceFormat(const vk::raii::SurfaceKHR &surface) const;

	[[nodiscard]] vk::raii::Device createDevice();
	[[nodiscard]] std::vector<std::pair<vk::raii::CommandPool, uint32_t>> createCommandPools();
	[[nodiscard]] VulkanSwapchainData createSwapchain();

	std::shared_ptr<class VulkanBackend> m_backend;

	vk::raii::PhysicalDevice m_physicalDevice;
	vk::raii::Device m_device;
	std::vector<std::pair<vk::raii::CommandPool, uint32_t>> m_commandPools;
	std::optional<VulkanSwapchainData> m_swapchain;
};

} // namespace dust::render::vulkan
