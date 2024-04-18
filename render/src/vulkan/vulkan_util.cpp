#include <algorithm>
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

} // namespace dust::render::vulkan
