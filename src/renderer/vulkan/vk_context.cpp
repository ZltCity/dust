#include "vk_context.hpp"

namespace dust::renderer::vulkan
{

const vk::raii::Context &context()
{
	static auto ctx = vk::raii::Context();

	return ctx;
}

} // namespace dust::renderer::vulkan