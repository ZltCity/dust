#include "vk_renderer.hpp"
#include "vk_context.hpp"

namespace dust::renderer::vulkan
{

vk_Renderer::vk_Renderer(bool debug)
	: m_appInfo(
		  applicationInfo().name, applicationInfo().version, engineInfo().name, engineInfo().version,
		  VK_API_VERSION_1_3),
	  m_enabledLayers(buildLayersList(debug)),
	  m_enabledExtensions(buildExtensionsList()),
	  m_instance(
		  context(), vk::InstanceCreateInfo(
						 {}, &m_appInfo, static_cast<uint32_t>(m_enabledLayers.size()), m_enabledLayers.data(),
						 static_cast<uint32_t>(m_enabledExtensions.size()), m_enabledExtensions.data()))
{}

std::vector<const char *> vk_Renderer::buildLayersList(bool debug)
{
	auto layers = std::vector<const char *> {};

	if (debug)
	{
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	return layers;
}

std::vector<const char *> vk_Renderer::buildExtensionsList()
{
	return {};
}

} // namespace dust::renderer::vulkan