#include <algorithm>
#include <deque>
#include <set>

#include "vulkan_util.hpp"

namespace dust::render::vulkan
{

std::vector<std::string> checkLayersAvailability(
	const std::vector<vk::LayerProperties> &layerProperties, const std::vector<const char *> &layerNames)
{
	auto layersSet = std::set<std::string_view> {};

	std::transform(
		layerProperties.begin(), layerProperties.end(), std::inserter(layersSet, layersSet.end()),
		[](const vk::LayerProperties &prop) { return static_cast<std::string_view>(prop.layerName); });

	auto missing = std::vector<std::string> {};

	for (const char *name : layerNames)
	{
		if (not layersSet.contains(std::string_view {name}))
		{
			missing.emplace_back(name);
		}
	}

	return missing;
}

std::vector<std::string> checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &extensionProperties, const std::vector<const char *> &extensionNames)
{
	auto extensionsSet = std::set<std::string_view> {};

	std::transform(
		extensionProperties.begin(), extensionProperties.end(), std::inserter(extensionsSet, extensionsSet.end()),
		[](const vk::ExtensionProperties &prop) { return static_cast<std::string_view>(prop.extensionName); });

	auto missing = std::vector<std::string> {};

	for (const char *name : extensionNames)
	{
		if (not extensionsSet.contains(std::string_view {name}))
		{
			missing.emplace_back(name);
		}
	}

	return missing;
}

std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> getSuitablePhysicalDevices(
	const vk::raii::Instance &instance, const std::optional<vk::raii::SurfaceKHR> &surface,
	const std::vector<vk::PhysicalDeviceType> &possibleTypes, const std::vector<vk::QueueFlagBits> &requiredQueueFlags)
{
	auto suitablePhysicalDevices = std::vector<std::pair<vk::raii::PhysicalDevice, uint32_t>> {};
	auto physicalDeviceIndex = uint32_t {};

	for (const auto &physicalDevice : instance.enumeratePhysicalDevices())
	{
		if (std::find(possibleTypes.begin(), possibleTypes.end(), physicalDevice.getProperties().deviceType) !=
				possibleTypes.end() and
			not getSuitableQueueFamilies(physicalDevice, surface, requiredQueueFlags).empty())
		{
			suitablePhysicalDevices.emplace_back(physicalDevice, physicalDeviceIndex);
		}

		++physicalDeviceIndex;
	}

	return suitablePhysicalDevices;
}

std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> getSuitableQueueFamilies(
	const vk::raii::PhysicalDevice &physicalDevice, const std::optional<vk::raii::SurfaceKHR> &surface,
	const std::vector<vk::QueueFlagBits> &requiredQueueFlags)
{
	auto queueFamilyIndex = uint32_t {};
	auto queueFlagBits = std::deque<vk::QueueFlagBits>(requiredQueueFlags.begin(), requiredQueueFlags.end());
	auto suitableQueueFamilies = std::vector<std::pair<vk::QueueFamilyProperties, uint32_t>> {};

	for (const auto &queueFamilyProperties : physicalDevice.getQueueFamilyProperties())
	{
		auto foundFlags = vk::QueueFlags {};

		while (not queueFlagBits.empty())
		{
			auto flag = queueFlagBits.front();

			if (queueFamilyProperties.queueFlags & flag)
			{
				foundFlags |= flag;
				queueFlagBits.pop_front();
			}
			else
			{
				break;
			}
		}

		if (foundFlags != vk::QueueFlagBits {} and
			(not surface.has_value() or physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, surface.value())))
		{
			suitableQueueFamilies.emplace_back(queueFamilyProperties, queueFamilyIndex);
		}

		++queueFamilyIndex;
	}

	return suitableQueueFamilies;
}

} // namespace dust::render::vulkan
