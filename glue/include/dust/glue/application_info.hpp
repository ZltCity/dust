#pragma once

#include <cstdint>
#include <string>

namespace dust::glue
{

struct ApplicationInfo
{
	std::string applicationName;
	uint32_t applicationVersion;
};

} // namespace dust::glue
