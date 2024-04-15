#pragma once

#include <optional>
#include <string>
#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render
{

class VulkanRenderer final : public Renderer
{
public:
	explicit VulkanRenderer(const glue::ApplicationInfo &applicationInfo, std::initializer_list<Hint> hints);

#if defined(WITH_SDL)
	VulkanRenderer(const glue::ApplicationInfo &applicationInfo, SDL_Window *window, std::initializer_list<Hint> hints);
#endif

private:
	struct SuitablePhysicalDevice
	{
		vk::raii::PhysicalDevice physicalDevice;
		uint32_t physicalDeviceIndex, deviceQueueFamily;
	};

	[[nodiscard]] static vk::raii::Instance createVulkanInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo);
	[[nodiscard]] static vk::raii::SurfaceKHR createVulkanSurface(const vk::raii::Instance &instance);
	[[nodiscard]] static vk::raii::Device createVulkanDevice(
		const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface, std::initializer_list<Hint> hints);

#if defined(WITH_SDL)
	[[nodiscard]] static vk::raii::Instance createVulkanInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
	[[nodiscard]] static vk::raii::SurfaceKHR createVulkanSurface(
		const vk::raii::Instance &instance, SDL_Window *window);
	[[nodiscard]] static std::vector<const char *> getRequiredVulkanExtensions(SDL_Window *window);
#endif

	[[nodiscard]] static std::vector<SuitablePhysicalDevice> getSuitablePhysicalDevices(
		const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface);
	[[nodiscard]] static std::optional<uint32_t> getSuitableQueueFamily(
		const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::SurfaceKHR &surface);

	static void checkLayersAvailability(
		const std::vector<vk::LayerProperties> &availableLayers, const std::vector<const char *> &requiredLayers);
	static void checkExtensionsAvailability(
		const std::vector<vk::ExtensionProperties> &availableExtensions,
		const std::vector<const char *> &requiredExtensions);

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	vk::raii::SurfaceKHR m_surface;
	vk::raii::Device m_device;
};

} // namespace dust::render
