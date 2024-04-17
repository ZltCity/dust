#include <algorithm>

#include <fmt/format.h>

#if defined(WITH_SDL)
#include <SDL_vulkan.h>
#endif

#include "vulkan_backend.hpp"
#include "vulkan_error.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

VulkanBackend::VulkanBackend(const glue::ApplicationInfo &applicationInfo)
	: m_context {}, m_instance {createVulkanInstance(m_context, applicationInfo)}
{}

#if defined(WITH_SDL)
VulkanBackend::VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
	: m_context {},
	  m_instance {createVulkanInstance(m_context, applicationInfo, window)},
	  m_surface {createVulkanSurface(m_instance, window)}
{}
#endif

std::unique_ptr<Renderer> VulkanBackend::createRenderer(std::initializer_list<Hint> hints) const
{
	const auto suitablePhysicalDevices = getSuitablePhysicalDevices();

	if (suitablePhysicalDevices.empty())
	{
		throw std::runtime_error {"Could not find suitable physical device."};
	}

	const auto physicalDevice = choosePhysicalDevice(suitablePhysicalDevices, hints);

	return m_surface.has_value() ? std::make_unique<VulkanRenderer>(
									   *m_surface, physicalDevice.physicalDevice,
									   physicalDevice.queueFamily.queueFamily, physicalDevice.queueFamily.queueCount)
								 : std::make_unique<VulkanRenderer>(
									   physicalDevice.physicalDevice, physicalDevice.queueFamily.queueFamily,
									   physicalDevice.queueFamily.queueCount);
}

std::vector<Device> VulkanBackend::getSuitableDevices() const
{
	const auto suitablePhysicalDevices = getSuitablePhysicalDevices();
	auto suitableDevices = std::vector<Device>(suitablePhysicalDevices.size());

	std::transform(
		suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(), suitableDevices.begin(),
		[](const SuitablePhysicalDevice &physicalDevice) {
			return Device {
				.name = physicalDevice.physicalDevice.getProperties().deviceName,
				.systemIndex = physicalDevice.physicalDeviceIndex};
		});

	return suitableDevices;
}

auto VulkanBackend::getSuitablePhysicalDevices() const -> std::vector<SuitablePhysicalDevice>
{
	auto suitablePhysicalDevices = std::vector<SuitablePhysicalDevice> {};
	auto physicalDeviceIndex = uint32_t {};

	for (const auto &physicalDevice : m_instance.enumeratePhysicalDevices())
	{
		const auto deviceProperties = physicalDevice.getProperties();
		const auto queueFamily = getSuitableQueueFamily(physicalDevice);

		if ((deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu or
			 deviceProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) and
			queueFamily.has_value())
		{
			suitablePhysicalDevices.push_back(SuitablePhysicalDevice {
				.physicalDevice = physicalDevice,
				.physicalDeviceIndex = physicalDeviceIndex,
				.queueFamily = *queueFamily});
		}

		++physicalDeviceIndex;
	}

	return suitablePhysicalDevices;
}

auto VulkanBackend::getSuitableQueueFamily(const vk::raii::PhysicalDevice &physicalDevice) const
	-> std::optional<SuitableQueueFamily>
{
	auto queueFamilyIndex = uint32_t {};

	for (const auto &queueFamilyProperties : physicalDevice.getQueueFamilyProperties())
	{
		if ((queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics) and
			(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eTransfer) and
			(queueFamilyProperties.queueFlags & vk::QueueFlagBits::eCompute) and
			(not m_surface.has_value() or physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, *(*m_surface))) and
			queueFamilyProperties.queueCount >= 1)
		{
			return {
				SuitableQueueFamily {.queueFamily = queueFamilyIndex, .queueCount = queueFamilyProperties.queueCount}};
		}

		++queueFamilyIndex;
	}

	return {};
}

vk::raii::Instance VulkanBackend::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo)
{
	throw HeadlessNotImplemented {};
}

#if defined(WITH_SDL)
vk::raii::Instance VulkanBackend::createVulkanInstance(
	const vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
{
	const auto vulkanApplicationInfo = vk::ApplicationInfo {
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

	if (const auto missing = getMissing(
			context.enumerateInstanceLayerProperties(), layers,
			[](const vk::LayerProperties &prop) { return static_cast<std::string_view>(prop.layerName); });
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following instance layers are not available: {}.", fmt::join(missing, ", "))};
	}

	const auto extensions = getRequiredVulkanExtensions(window);

	if (const auto missing = getMissing(
			context.enumerateInstanceExtensionProperties(), extensions,
			[](const vk::ExtensionProperties &prop) { return static_cast<std::string_view>(prop.extensionName); });
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following instance extensions are not available: {}.", fmt::join(missing, ", "))};
	}

	return vk::raii::Instance {context, vk::InstanceCreateInfo {{}, &vulkanApplicationInfo, layers, extensions}};
}

vk::raii::SurfaceKHR VulkanBackend::createVulkanSurface(const vk::raii::Instance &instance, SDL_Window *window)
{
	auto surface = VkSurfaceKHR {};

	if (not SDL_Vulkan_CreateSurface(window, *instance, &surface))
	{
		throw std::runtime_error {"Could not create Vulkan surface."};
	}

	return vk::raii::SurfaceKHR {instance, surface};
}

std::vector<const char *> VulkanBackend::getRequiredVulkanExtensions(SDL_Window *window)
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

auto VulkanBackend::choosePhysicalDevice(
	const std::vector<SuitablePhysicalDevice> &suitablePhysicalDevices, std::initializer_list<Hint> hints)
	-> SuitablePhysicalDevice
{
	const auto useDeviceHint =
		std::find_if(hints.begin(), hints.end(), [](const Hint &hint) { return hint.name == HintName::UseDevice; });

	if (useDeviceHint == hints.end())
	{
		return suitablePhysicalDevices.front();
	}
	else
	{
		const auto testDevice = [&useDeviceHint](const SuitablePhysicalDevice &device) {
			auto result = false;

			std::visit(
				[&result, &device](auto &&value) {
					using T = std::decay_t<decltype(value)>;

					if constexpr (std::is_same_v<T, int32_t> or std::is_same_v<T, float>)
						result = static_cast<uint32_t>(value) == device.physicalDeviceIndex;
					if constexpr (std::is_same_v<T, std::string>)
						result = value == device.physicalDevice.getProperties().deviceName;
				},
				useDeviceHint->value);

			return result;
		};
		const auto physicalDevice = std::find_if(
			suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(),
			[&testDevice](const SuitablePhysicalDevice &device) { return testDevice(device); });

		return physicalDevice != suitablePhysicalDevices.end() ? *physicalDevice : suitablePhysicalDevices.front();
	}
}

} // namespace dust::render::vulkan
