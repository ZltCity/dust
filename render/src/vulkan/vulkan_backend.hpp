#pragma once

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

	[[nodiscard]] const std::optional<vk::raii::SurfaceKHR> &getSurface() const;
	[[nodiscard]] std::vector<Device> getSuitableDevices() const final;

	[[nodiscard]] std::shared_ptr<Renderer> createRenderer(const std::vector<Hint> &hints) final;

private:
	[[nodiscard]] std::vector<const char *> getRequiredInstanceLayers() const;
#if defined(WITH_SDL)
	[[nodiscard]] std::vector<const char *> getRequiredInstanceExtensions(SDL_Window *window) const;
#endif

	[[nodiscard]] vk::raii::PhysicalDevice choosePhysicalDevice(
		const std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> &suitablePhysicalDevices,
		const std::vector<Hint> &hints) const;

	[[nodiscard]] vk::raii::Instance createInstance(
		const glue::ApplicationInfo &applicationInfo, const std::vector<const char *> &requiredLayers,
		const std::vector<const char *> &requiredExtensions);
#if defined(WITH_SDL)
	[[nodiscard]] vk::raii::SurfaceKHR createSurface(SDL_Window *window);
#endif

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
	std::optional<vk::raii::SurfaceKHR> m_surface;
};

} // namespace dust::render::vulkan
