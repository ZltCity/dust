#include "renderer.hpp"
#include "vulkan/vk_renderer.hpp"

namespace dust::renderer
{

Renderer::~Renderer() noexcept = default;

std::unique_ptr<Renderer> Renderer::create(bool debug)
{
	return std::make_unique<vulkan::vk_Renderer>(debug);
}

} // namespace dust::renderer