#pragma once

#include <vector>

namespace dust::render
{

enum class Backend
{
	Vulkan = 0
};

[[nodiscard]] std::vector<Backend> getSupportedBackends();

} // namespace dust::render
