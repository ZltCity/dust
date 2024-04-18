#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace dust::render::vulkan
{

[[nodiscard]] std::vector<std::string> checkLayersAvailability(
	const std::vector<vk::LayerProperties> &layerProperties, const std::vector<const char *> &layerNames);
[[nodiscard]] std::vector<std::string> checkExtensionsAvailability(
	const std::vector<vk::ExtensionProperties> &extensionProperties, const std::vector<const char *> &extensionNames);

} // namespace dust::render::vulkan
