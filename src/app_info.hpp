#pragma once

#include <cstdint>

namespace dust
{

struct ApplicationInfo
{
	const char *name = DUST_APP_NAME;
	uint32_t version = DUST_APP_VERSION;
};

[[nodiscard]] const ApplicationInfo &applicationInfo();

} // namespace dust