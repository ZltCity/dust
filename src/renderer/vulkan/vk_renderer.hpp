#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

#include "../../app_info.hpp"
#include "../../engine_info.hpp"
#include "../renderer.hpp"

namespace dust::renderer::vulkan
{

class vk_Renderer final : public Renderer
{
public:
	explicit vk_Renderer(bool debug);

private:
	[[nodiscard]] static std::vector<const char *> buildLayersList(bool debug);
	[[nodiscard]] static std::vector<const char *> buildExtensionsList();

	const vk::ApplicationInfo m_appInfo;
	const std::vector<const char *> m_enabledLayers;
	const std::vector<const char *> m_enabledExtensions;

	vk::raii::Instance m_instance;
};

} // namespace dust::renderer::vulkan