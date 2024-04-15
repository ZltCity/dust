#include <algorithm>
#include <array>
#include <stdexcept>

#include <fmt/format.h>

#if defined(WITH_SDL)
#include <SDL_vulkan.h>
#endif

#include "vulkan_renderer.hpp"

namespace dust::render
{

class NotImplemented_Vulkan final : public std::runtime_error
{
public:
	NotImplemented_Vulkan() : std::runtime_error {"Vulkan headless rendering is not implemented."}
	{}
};

VulkanRenderer::VulkanRenderer(const glue::ApplicationInfo &applicationInfo, std::initializer_list<Hint> hints)
	: m_context {},
	  m_instance {createVulkanInstance(m_context, applicationInfo)},
	  m_surface {createVulkanSurface(m_instance)},
	  m_device {createVulkanDevice(m_instance, m_surface, hints)}
{}

#if defined(WITH_SDL)
VulkanRenderer::VulkanRenderer(
	const glue::ApplicationInfo &applicationInfo, SDL_Window *window, std::initializer_list<Hint> hints)
	: m_context {},
	  m_instance {createVulkanInstance(m_context, applicationInfo, window)},
	  m_surface {createVulkanSurface(m_instance, window)},
	  m_device {createVulkanDevice(m_instance, m_surface, hints)}
{}

vk::raii::Instance VulkanRenderer::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
{
	const auto vkAppInfo = vk::ApplicationInfo {
		applicationInfo.applicationName.c_str(),
		applicationInfo.applicationVersion,
		DUST_ENGINE_NAME,
		DUST_ENGINE_VERSION,
		VK_API_VERSION_1_3,
	};
	const auto layers = std::vector<const char *>
	{
#if !defined(NDEBUG)
		"VK_LAYER_KHRONOS_validation",
#endif
	};
	const auto extensions = getRequiredVulkanExtensions(window);

	checkLayersAvailability(context.enumerateInstanceLayerProperties(), layers);
	checkExtensionsAvailability(context.enumerateInstanceExtensionProperties(), extensions);

	return vk::raii::Instance {context, vk::InstanceCreateInfo {{}, &vkAppInfo, layers, extensions}};
}

vk::raii::SurfaceKHR VulkanRenderer::createVulkanSurface(const vk::raii::Instance &instance, SDL_Window *window)
{
	auto surface = VkSurfaceKHR {};

	if (not SDL_Vulkan_CreateSurface(window, *instance, &surface))
	{
		throw std::runtime_error {"Could not create Vulkan surface."};
	}

	return vk::raii::SurfaceKHR {instance, surface};
}

std::vector<const char *> VulkanRenderer::getRequiredVulkanExtensions(SDL_Window *window)
{
	unsigned int count = 0;

	if (not SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr))
	{
		throw std::runtime_error {"'SDL_Vulkan_GetInstanceExtensions' failed on get extensions count."};
	}

	auto extensions = std::vector<const char *>(count, nullptr);

	if (not SDL_Vulkan_GetInstanceExtensions(window, &count, extensions.data()))
	{
		throw std::runtime_error {"'SDL_Vulkan_GetInstanceExtensions' failed on get extensions names."};
	}

	return extensions;
}

#endif

vk::raii::Instance VulkanRenderer::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo)
{
	throw NotImplemented_Vulkan {};
}

vk::raii::SurfaceKHR VulkanRenderer::createVulkanSurface(const vk::raii::Instance &instance)
{
	throw NotImplemented_Vulkan {};
}

vk::raii::Device VulkanRenderer::createVulkanDevice(
	const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface, std::initializer_list<Hint> hints)
{
	const auto suitablePhysicalDevices = getSuitablePhysicalDevices(instance, surface);

	if (suitablePhysicalDevices.empty())
	{
		throw std::runtime_error {"Could not find suitable Vulkan physical device."};
	}

	const auto useDeviceHintIter =
		std::find_if(hints.begin(), hints.end(), [](const Hint &hint) { return hint.first == HintName::UseDevice; });
	const auto physicalDeviceIter = std::find_if(
		suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(),
		[&useDeviceHintIter, &hints](const SuitablePhysicalDevice &device) {
			return useDeviceHintIter != hints.end() and
				   useDeviceHintIter->second == static_cast<int32_t>(device.physicalDeviceIndex);
		});
	const auto &physicalDevice =
		physicalDeviceIter != suitablePhysicalDevices.end() ? *physicalDeviceIter : suitablePhysicalDevices.front();
	const auto queuePriorities = std::array {1.f};
	const auto deviceQueueCreateInfo =
		std::array {vk::DeviceQueueCreateInfo {{}, physicalDevice.deviceQueueFamily, queuePriorities}};

	return vk::raii::Device {physicalDevice.physicalDevice, vk::DeviceCreateInfo {{}, deviceQueueCreateInfo}};
}

auto VulkanRenderer::getSuitablePhysicalDevices(const vk::raii::Instance &instance, const vk::raii::SurfaceKHR &surface)
	-> std::vector<SuitablePhysicalDevice>
{
	const auto physicalDevices = instance.enumeratePhysicalDevices();
	auto suitablePhysicalDevices = std::vector<SuitablePhysicalDevice> {};

	for (auto i = 0; i < physicalDevices.size(); ++i)
	{
		const auto &physicalDevice = physicalDevices.at(i);
		const auto queueFamily = getSuitableQueueFamily(physicalDevice, surface);

		if (physicalDevice.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu and
			queueFamily.has_value())
		{
			suitablePhysicalDevices.emplace_back(physicalDevice, static_cast<uint32_t>(i), queueFamily.value());
		}
	}

	return suitablePhysicalDevices;
}

std::optional<uint32_t> VulkanRenderer::getSuitableQueueFamily(
	const vk::raii::PhysicalDevice &physicalDevice, const vk::raii::SurfaceKHR &surface)
{
	const auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	for (auto i = uint32_t {}; i < queueFamilyProperties.size(); ++i)
	{
		const auto &prop = queueFamilyProperties.at(i);

		if ((prop.queueFlags & vk::QueueFlagBits::eGraphics) and (prop.queueFlags & vk::QueueFlagBits::eTransfer) and
			(prop.queueFlags & vk::QueueFlagBits::eCompute) and physicalDevice.getSurfaceSupportKHR(i, *surface))
		{
			return {i};
		}
	}

	return {};
}

void VulkanRenderer::checkLayersAvailability(
	const std::vector<vk::LayerProperties> &availableLayers, const std::vector<const char *> &requiredLayers)
{
	for (auto layerName : requiredLayers)
	{
		if (std::find_if(availableLayers.begin(), availableLayers.end(), [layerName](auto &&prop) {
				return static_cast<std::string_view>(prop.layerName) == std::string_view {layerName};
			}) == availableLayers.end())
		{
			throw std::runtime_error {fmt::format("Layer '{}' is unavailable.", layerName)};
		}
	}
}

void VulkanRenderer::checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &availableExtensions,
	const std::vector<const char *> &requiredExtensions)
{
	for (auto extensionName : requiredExtensions)
	{
		if (std::find_if(availableExtensions.begin(), availableExtensions.end(), [extensionName](auto &&prop) {
				return static_cast<std::string_view>(prop.extensionName) == std::string_view {extensionName};
			}) == availableExtensions.end())
		{
			throw std::runtime_error {fmt::format("Extension '{}' is unavailable.", extensionName)};
		}
	}
}

} // namespace dust::render
