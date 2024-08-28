#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace dust::renderer::vulkan
{

[[nodiscard]] const vk::raii::Context &context();

}