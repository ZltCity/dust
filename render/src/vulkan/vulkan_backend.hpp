#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/backend.hpp>

#include "vulkan_helper_struct.hpp"

namespace dust::render::vulkan
{

class VulkanBackend final : public Backend, public std::enable_shared_from_this<VulkanBackend>
{
public:
#if defined(WITH_SDL)
	VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
#endif

	[[nodiscard]] std::shared_ptr<Renderer> createRenderer(const std::vector<Hint> &hints) final;

	[[nodiscard]] std::vector<Device> getSuitableDevices() const final;

private:
	[[nodiscard]] vk::QueueFlags getRequiredQueueFlags() const;
	[[nodiscard]] std::vector<const char *> getRequiredDeviceExtensions() const;
	[[nodiscard]] std::vector<SuitablePhysicalDevice> getSuitablePhysicalDevices(
		vk::QueueFlags queueFlags, bool needPresentSupport = true) const;
	[[nodiscard]] std::vector<SuitableQueueFamily> getSuitableQueueFamilies(
		const vk::raii::PhysicalDevice &physicalDevice, bool needPresentSupport = true) const;
	[[nodiscard]] bool getPresentSupport(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily) const;

	[[nodiscard]] static std::vector<const char *> getRequiredInstanceLayers();
#if defined(WITH_SDL)
	[[nodiscard]] static std::vector<const char *> getRequiredInstanceExtensions(SDL_Window *window);
#endif

	[[nodiscard]] static SuitablePhysicalDevice choosePhysicalDevice(
		const std::vector<SuitablePhysicalDevice> &suitablePhysicalDevices, const std::vector<Hint> &hints);

	[[nodiscard]] static vk::raii::Instance createInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo,
		const std::vector<const char *> &requiredLayers, const std::vector<const char *> &requiredExtensions);
#if defined(WITH_SDL)
	[[nodiscard]] static vk::raii::SurfaceKHR createSurface(const vk::raii::Instance &instance, SDL_Window *window);
#endif
	[[nodiscard]] static vk::raii::Device createDevice(
		const vk::raii::PhysicalDevice &physicalDevice, const std::vector<SuitableQueueFamily> &queueFamilies,
		const std::vector<const char *> &requiredExtensions);

	const std::vector<vk::QueueFlagBits> requiredQueueFlagBits = {
		vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer};

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
