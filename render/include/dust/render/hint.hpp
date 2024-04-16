#pragma once

#include <string>
#include <utility>
#include <variant>

namespace dust::render
{

enum class HintName
{
	UseDevice = 0
};

struct Hint
{
	HintName name;
	std::variant<bool, int32_t, float, std::string> value;
};

} // namespace dust::render
