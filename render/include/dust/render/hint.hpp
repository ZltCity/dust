#pragma once

#include <utility>

namespace dust::render
{

enum class HintName
{
	UseDevice = 0
};

using Hint = std::pair<HintName, int32_t>;

} // namespace dust::render
