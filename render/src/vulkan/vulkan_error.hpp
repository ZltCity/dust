#pragma once

namespace dust::render::vulkan
{

class HeadlessNotImplemented final : public std::runtime_error
{
public:
	HeadlessNotImplemented() : std::runtime_error {"Vulkan headless rendering has not yet been implemented."}
	{}
};

} // namespace dust::render::vulkan
