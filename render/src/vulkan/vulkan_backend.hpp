#pragma once

#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/backend.hpp>

namespace dust::render::vulkan
{

class VulkanBackend final : public Backend
{
public:
	explicit VulkanBackend(const glue::ApplicationInfo &applicationInfo);

#if defined(WITH_SDL)
	VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window);
#endif

	[[nodiscard]] std::unique_ptr<Renderer> createRenderer(std::initializer_list<Hint> hints) const final;

	[[nodiscard]] std::vector<Device> getSuitableDevices() const final;

private:
	struct SuitableQueueFamily
	{
		uint32_t queueFamily, queueCount;
	};

	struct SuitablePhysicalDevice
	{
		vk::raii::PhysicalDevice physicalDevice;
		uint32_t physicalDeviceIndex;
		SuitableQueueFamily queueFamily;
	};

	[[nodiscard]] std::vector<SuitablePhysicalDevice> getSuitablePhysicalDevices() const;
	[[nodiscard]] std::optional<SuitableQueueFamily> getSuitableQueueFamily(
		const vk::raii::PhysicalDevice &physicalDevice) const;

	[[nodiscard]] static SuitablePhysicalDevice choosePhysicalDevice(
		const std::vector<SuitablePhysicalDevice> &suitablePhysicalDevices, const std::vector<Hint> &hints);

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
