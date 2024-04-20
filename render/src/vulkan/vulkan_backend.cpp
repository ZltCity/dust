#include <algorithm>
#include <iterator>
#include <list>
#include <numeric>

#include <fmt/format.h>

#if defined(WITH_SDL)
#include <SDL_vulkan.h>
#endif

#include "vulkan_backend.hpp"
#include "vulkan_renderer.hpp"
#include "vulkan_util.hpp"
#include "vulkan_window_swapchain.hpp"

namespace dust::render::vulkan
{

[[nodiscard]] static std::vector<const char *> getRequiredInstanceLayers()
{
	return std::vector<const char *>
	{
#if !defined(NDEBUG)
		"VK_LAYER_KHRONOS_validation",
#endif
	};
}

#if defined(WITH_SDL)
[[nodiscard]] static std::vector<const char *> getRequiredInstanceExtensions(SDL_Window *window)
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

[[nodiscard]] static auto getIndexedQueueFamilyProperties(
	const std::vector<vk::QueueFamilyProperties> &queueFamilyProperties)
{
	auto queueFamilyIndex = uint32_t {};
	auto indexedQueueFamilyProperties = std::list<std::pair<vk::QueueFamilyProperties, uint32_t>> {};

	std::transform(
		queueFamilyProperties.begin(), queueFamilyProperties.end(), std::back_inserter(indexedQueueFamilyProperties),
		[&queueFamilyIndex](const vk::QueueFamilyProperties &prop) {
			return std::make_pair(prop, queueFamilyIndex++);
		});

	return indexedQueueFamilyProperties;
}

[[nodiscard]] static SuitablePhysicalDevice choosePhysicalDevice(
	const std::vector<SuitablePhysicalDevice> &suitablePhysicalDevices, const std::vector<Hint> &hints)
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
						result = static_cast<uint32_t>(value) == device.deviceIndex;
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

[[nodiscard]] static vk::raii::Instance createInstance(
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
[[nodiscard]] static vk::raii::SurfaceKHR createSurface(const vk::raii::Instance &instance, SDL_Window *window)
{
	auto surface = VkSurfaceKHR {};

	if (not SDL_Vulkan_CreateSurface(window, *instance, &surface))
	{
		throw std::runtime_error {"Could not create Vulkan surface."};
	}

	return vk::raii::SurfaceKHR {instance, surface};
}
#endif

[[nodiscard]] vk::raii::Device createDevice(
	const vk::raii::PhysicalDevice &physicalDevice, const std::vector<SuitableQueueFamily> &queueFamilies,
	const std::vector<const char *> &requiredExtensions)
{
	if (const auto missing =
			checkExtensionsAvailability(physicalDevice.enumerateDeviceExtensionProperties(), requiredExtensions);
		not missing.empty())
	{
		throw std::runtime_error {
			fmt::format("Following device extensions are not available: {}.", fmt::join(missing, ", "))};
	}

	auto queueCreateInfos = std::vector<vk::DeviceQueueCreateInfo> {};
	auto queuePriorities = std::vector<std::vector<float>> {};

	for (const auto &queueFamily : queueFamilies)
	{
		queuePriorities.emplace_back(queueFamily.queueCount, 1.0f);
		queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags {}, queueFamily.queueFamily, queuePriorities.back());
	}

	return vk::raii::Device {physicalDevice, vk::DeviceCreateInfo {{}, queueCreateInfos, {}, requiredExtensions}};
}

#if defined(WITH_SDL)
VulkanBackend::VulkanBackend(const glue::ApplicationInfo &applicationInfo, SDL_Window *window)
	: m_context {},
	  m_instance {createInstance(
		  m_context, applicationInfo, getRequiredInstanceLayers(), getRequiredInstanceExtensions(window))},
	  m_surface {createSurface(m_instance, window)}
{}
#endif

std::shared_ptr<Renderer> VulkanBackend::createRenderer(const std::vector<Hint> &hints) const
{
	const auto suitablePhysicalDevices = getSuitablePhysicalDevices(getRequiredQueueFlags());

	if (suitablePhysicalDevices.empty())
	{
		throw std::runtime_error {"Could not find suitable physical device."};
	}

	const auto physicalDevice = choosePhysicalDevice(suitablePhysicalDevices, hints).physicalDevice;
	auto queueFamilies = chooseQueueFamilies(physicalDevice, requiredQueueFlagBits);

	return std::make_shared<VulkanRenderer>(
		createDevice(physicalDevice, queueFamilies, getRequiredDeviceExtensions()), std::move(queueFamilies));
}

std::vector<Device> VulkanBackend::getSuitableDevices() const
{
	const auto suitablePhysicalDevices = getSuitablePhysicalDevices(getRequiredQueueFlags());
	auto suitableDevices = std::vector<Device>(suitablePhysicalDevices.size());

	std::transform(
		suitablePhysicalDevices.begin(), suitablePhysicalDevices.end(), suitableDevices.begin(),
		[](const SuitablePhysicalDevice &device) {
			return Device {.name = device.physicalDevice.getProperties().deviceName, .systemIndex = device.deviceIndex};
		});

	return suitableDevices;
}

vk::QueueFlags VulkanBackend::getRequiredQueueFlags() const
{
	return std::accumulate(
		requiredQueueFlagBits.begin(), requiredQueueFlagBits.end(), vk::QueueFlags {},
		[](vk::QueueFlags a, vk::QueueFlags b) { return a | b; });
}

std::vector<const char *> VulkanBackend::getRequiredDeviceExtensions() const
{
	auto extensions = std::vector<const char *> {};

	if (m_surface.has_value())
	{
		extensions.push_back("VK_KHR_swapchain");
	}

	return extensions;
}

std::vector<SuitablePhysicalDevice> VulkanBackend::getSuitablePhysicalDevices(
	vk::QueueFlags queueFlags, bool presentSupport) const
{
	auto suitablePhysicalDevices = std::vector<SuitablePhysicalDevice> {};
	auto physicalDeviceIndex = uint32_t {};

	for (const auto &physicalDevice : m_instance.enumeratePhysicalDevices())
	{
		const auto deviceProperties = physicalDevice.getProperties();

		if ((deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu or
			 deviceProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu))
		{
			auto queueFamilyIndex = uint32_t {};
			auto foundFlags = vk::QueueFlags {};
			auto hasPresentSupport = false;

			for (const auto &queueFamilyProperties : physicalDevice.getQueueFamilyProperties())
			{
				foundFlags |= queueFamilyProperties.queueFlags;
				hasPresentSupport = checkPresentSupport(physicalDevice, queueFamilyIndex);

				++queueFamilyIndex;
			}

			if ((foundFlags & queueFlags) == queueFlags and (not presentSupport or hasPresentSupport))
			{
				suitablePhysicalDevices.emplace_back(physicalDevice, physicalDeviceIndex);
			}
		}

		++physicalDeviceIndex;
	}

	return suitablePhysicalDevices;
}

std::vector<SuitableQueueFamily> VulkanBackend::chooseQueueFamilies(
	const vk::raii::PhysicalDevice &physicalDevice, const std::vector<vk::QueueFlagBits> &queueFlagBits) const
{
	auto queueFamilyProperties = getIndexedQueueFamilyProperties(physicalDevice.getQueueFamilyProperties());
	auto queueFamilies = std::vector<SuitableQueueFamily> {};

	for (auto flagBit : queueFlagBits)
	{
		for (auto propIt = queueFamilyProperties.begin(); propIt != queueFamilyProperties.end(); ++propIt)
		{
			const auto [queueFamily, familyIndex] = std::move(*propIt);

			if (queueFamily.queueFlags & flagBit)
			{
				queueFamilies.emplace_back(
					familyIndex, queueFamily.queueCount, vk::QueueFlags {flagBit},
					checkPresentSupport(physicalDevice, familyIndex));

				queueFamilyProperties.erase(propIt);
				queueFamilyProperties.emplace_back(queueFamily, familyIndex);

				break;
			}
		}
	}

	return queueFamilies;
}

bool VulkanBackend::checkPresentSupport(const vk::raii::PhysicalDevice &physicalDevice, uint32_t queueFamily) const
{
	return m_surface.has_value() and physicalDevice.getSurfaceSupportKHR(queueFamily, *(m_surface.value()));
}

} // namespace dust::render::vulkan
