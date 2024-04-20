#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/backend.hpp>

#include "vulkan_basic_swapchain.hpp"
#include "vulkan_helper_struct.hpp"

namespace dust::render::vulkan
{

class VulkanBackend final : public Backend, public std::enable_shared_from_this<VulkanBackend>
{
public:
#if defined(WITH_SDL)
	VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
#endif

	[[nodiscard]] std::shared_ptr<Renderer> createRenderer(const std::vector<Hint> &hints) const final;

	[[nodiscard]] std::vector<Device> getSuitableDevices() const final;

private:
	[[nodiscard]] vk::QueueFlags getRequiredQueueFlags() const;
	[[nodiscard]] std::vector<const char *> getRequiredDeviceExtensions() const;
	[[nodiscard]] std::vector<SuitablePhysicalDevice> getSuitablePhysicalDevices(
		vk::QueueFlags queueFlags, bool presentSupport = true) const;
	[[nodiscard]] std::vector<SuitableQueueFamily> chooseQueueFamilies(
		const vk::raii::PhysicalDevice &physicalDevice, const std::vector<vk::QueueFlagBits> &queueFlagBits) const;
	[[nodiscard]] bool checkPresentSupport(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily) const;

	const std::vector<vk::QueueFlagBits> requiredQueueFlagBits = {
		vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer};

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
