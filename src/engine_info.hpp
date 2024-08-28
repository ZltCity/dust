#pragma once

#include <cstdint>

namespace dust
{

struct EngineInfo
{
	const char *name = "Dust";
	uint32_t version = 1;
};

[[nodiscard]] const EngineInfo &engineInfo();

} // namespace dust