#include <fmt/format.h>

#if defined(WITH_SDL)
#include <SDL_vulkan.h>
#endif

#include "vulkan_backend.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

#if defined(WITH_SDL)
VulkanBackend::VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
	: m_context {},
	  m_instance {createInstance(
		  m_context, applicationInfo, getRequiredInstanceLayers(), getRequiredInstanceExtensions(window))},
	  m_surface {createSurface(m_instance, window)}
{}
#endif

std::shared_ptr<Renderer> VulkanBackend::createRenderer(const std::vector<Hint> &hints)
{
	const auto suitablePhysicalDevices =
		getSuitablePhysicalDevices(m_instance, m_surface, possibleDeviceTypes, requiredQueueFlags);

	if (suitablePhysicalDevices.empty())
	{
		throw std::runtime_error {"Could not find suitable physical device."};
	}

	return std::make_shared<VulkanRenderer>(choosePhysicalDevice(suitablePhysicalDevices, hints), shared_from_this());
}

std::vector<Device> VulkanBackend::getSuitableDevices() const
{
	const auto suitablePhysicalDevices =
		getSuitablePhysicalDevices(m_instance, m_surface, possibleDeviceTypes, requiredQueueFlags);
	auto suitableDevices = std::vector<Device>(suitablePhysicalDevices.size());

	std::transform(
		suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(), suitableDevices.begin(),
		[](const std::pair<vk::raii::PhysicalDevice, uint32_t> &device) {
			return Device {.name = std::get<0>(device).getProperties().deviceName, .systemIndex = std::get<1>(device)};
		});

	return suitableDevices;
}

const std::optional<vk::raii::SurfaceKHR> &VulkanBackend::getSurface() const
{
	return m_surface;
}

std::vector<const char *> VulkanBackend::getRequiredInstanceLayers()
{
	return std::vector<const char *>
	{
#if !defined(NDEBUG)
		"VK_LAYER_KHRONOS_validation",
#endif
	};
}

#if defined(WITH_SDL)
std::vector<const char *> VulkanBackend::getRequiredInstanceExtensions(SDL_Window *window)
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

vk::raii::PhysicalDevice VulkanBackend::choosePhysicalDevice(
	const std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> &suitablePhysicalDevices,
	const std::vector<Hint> &hints)
{
	const auto useDeviceHint =
		std::find_if(hints.begin(), hints.end(), [](const Hint &hint) { return hint.name == HintName::UseDevice; });

	if (useDeviceHint == hints.end())
	{
		return std::get<0>(suitablePhysicalDevices.front());
	}
	else
	{
		const auto testDevice = [&useDeviceHint](const std::pair<vk::raii::PhysicalDevice, uint32_t> &device) {
			auto result = false;

			std::visit(
				[&result, &device](auto &&value) {
					using T = std::decay_t<decltype(value)>;

					if constexpr (std::is_same_v<T, int32_t> or std::is_same_v<T, float>)
						result = static_cast<uint32_t>(value) == std::get<1>(device);
					if constexpr (std::is_same_v<T, std::string>)
						result = value == std::get<0>(device).getProperties().deviceName;
				},
				useDeviceHint->value);

			return result;
		};
		const auto physicalDevice = std::find_if(
			suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(),
			[&testDevice](const std::pair<vk::raii::PhysicalDevice, uint32_t> &device) { return testDevice(device); });

		return std::get<0>(
			physicalDevice != suitablePhysicalDevices.end() ? *physicalDevice : suitablePhysicalDevices.front());
	}
}

vk::raii::Instance VulkanBackend::createInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo,
	const std::vector<const char *> &requiredLayers, const std::vector<const char *> &requiredExtensions)
{
	const auto vulkanApplicationInfo = vk::ApplicationInfo {
		applicationInfo.applicationName.c_str(),
		applicationInfo.applicationVersion,
		DUST_ENGINE_NAME,
		DUST_ENGINE_VERSION,
		VK_API_VERSION_1_3,
	};

	if (const auto missing = checkLayersAvailability(context.enumerateInstanceLayerProperties(), requiredLayers);
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following instance layers are not available: {}.", fmt::join(missing, ", "))};
	}

	if (const auto missing =
			checkExtensionsAvailability(context.enumerateInstanceExtensionProperties(), requiredExtensions);
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following instance extensions are not available: {}.", fmt::join(missing, ", "))};
	}

	return vk::raii::Instance {
		context, vk::InstanceCreateInfo {{}, &vulkanApplicationInfo, requiredLayers, requiredExtensions}};
}

#if defined(WITH_SDL)
vk::raii::SurfaceKHR VulkanBackend::createSurface(const vk::raii::Instance &instance, SDL_Window *window)
{
	auto surface = VkSurfaceKHR {};

	if (not SDL_Vulkan_CreateSurface(window, *instance, &surface))
	{
		throw std::runtime_error {"Could not create Vulkan surface."};
	}

	return vk::raii::SurfaceKHR {instance, surface};
}
#endif

} // namespace dust::render::vulkan
