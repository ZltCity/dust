#pragma once

#include <memory>
#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/backend.hpp>

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
	[[nodiscard]] std::vector<const char *> getRequiredDeviceExtensions() const;

	[[nodiscard]] static std::vector<const char *> getRequiredInstanceLayers();
#if defined(WITH_SDL)
	[[nodiscard]] static std::vector<const char *> getRequiredInstanceExtensions(SDL_Window *window);
#endif

	[[nodiscard]] static std::pair<vk::raii::PhysicalDevice, uint32_t> choosePhysicalDevice(
		const std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> &suitablePhysicalDevices,
		const std::vector<Hint> &hints);

	[[nodiscard]] static vk::raii::Instance createInstance(
		const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo,
		const std::vector<const char *> &requiredLayers, const std::vector<const char *> &requiredExtensions);
#if defined(WITH_SDL)
	[[nodiscard]] static vk::raii::SurfaceKHR createSurface(const vk::raii::Instance &instance, SDL_Window *window);
#endif
	[[nodiscard]] static vk::raii::Device createDevice(
		const vk::raii::PhysicalDevice &physicalDevice, const std::vector<SuitableQueueFamily> &queueFamilies,
		const std::vector<const char *> &requiredExtensions);

	const std::vector<vk::PhysicalDeviceType> possibleDeviceTypes = {
		vk::PhysicalDeviceType::eIntegratedGpu, vk::PhysicalDeviceType::eDiscreteGpu};
	const std::vector<vk::QueueFlagBits> requiredQueueFlags = {
		vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer};

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
