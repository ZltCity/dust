#pragma once

namespace dust::render::vulkan
{

class HeadlessNotImplemented final : public std::logic_error
{
public:
	HeadlessNotImplemented() : std::logic_error {"Vulkan headless rendering has not yet been implemented."}
	{}
};

class NoValidSurface final : public std::logic_error
{
public:
	NoValidSurface() : std::logic_error {"Backend does not contain a valid surface."}
	{}
};

} // namespace dust::render::vulkan
