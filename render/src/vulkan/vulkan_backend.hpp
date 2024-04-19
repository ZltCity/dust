#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/backend.hpp>

#include "vulkan_basic_swapchain.hpp"

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
	using SuitablePhysicalDevice = std::pair<vk::raii::PhysicalDevice, uint32_t>;

	[[nodiscard]] std::vector<SuitablePhysicalDevice> getSuitablePhysicalDevices(
		vk::QueueFlags queueFlags = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer,
		bool presentSupport = true) const;

	[[nodiscard]] static SuitablePhysicalDevice choosePhysicalDevice(
		const std::vector<SuitablePhysicalDevice> &suitablePhysicalDevices, const std::vector<Hint> &hints);

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
