#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <dust/render/renderer.hpp>

namespace dust::render
{

class VulkanRenderer final : public Renderer
{
public:
	explicit VulkanRenderer(const glue::ApplicationInfo &applicationInfo);

private:
	[[nodiscard]] static vk::raii::Instance createVulkanInstance(
		vk::raii::Context &context, const glue::ApplicationInfo &applicationInfo);

	vk::raii::Context m_context;
	vk::raii::Instance m_instance;
};

} // namespace dust::render
