#pragma once

#include <cstdint>
#include <string>

namespace dust::render
{

struct Device
{
	std::string name;
	uint32_t systemIndex;
};

} // namespace dust::render
