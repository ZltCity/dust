#include "vulkan_renderer.hpp"

namespace dust::render
{

VulkanRenderer::VulkanRenderer(const glue::ApplicationInfo &applicationInfo)
	: m_context {}, m_instance {createVulkanInstance(m_context)}
{}

vk::raii::Instance VulkanRenderer::createVulkanInstance(
	vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo)
{
	static auto vkAppInfo = vk::ApplicationInfo {
		applicationInfo.applicationName.c_str(), applicationInfo.applicationVersion, "Dust",
		applicationInfo.dustVersion, VK_MAKE_API_VERSION(0, 1, 3, 0)};

	return vk::raii::Instance {context, vk::InstanceCreateInfo {vk::InstanceCreateFlags {}, &applicationInfo, }};
}

} // namespace dust::render
