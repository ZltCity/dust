#pragma once

#include <algorithm>
#include <optional>

namespace dust::render::vulkan
{

template<class AvailableT, class RequiredT, class CompT>
std::optional<RequiredT> getMissing(
	const std::vector<AvailableT> &available, const std::vector<RequiredT> &required,
	CompT compare = std::equal_to<AvailableT>())
{
	for (const auto &reqValue : required)
	{
		if (std::find_if(available.begin(), available.end(), [&reqValue, &compare](auto &&availValue) {
				return compare(availValue, reqValue);
			}) == available.end())
		{
			return {reqValue};
		}
	}

	return {};
}

} // namespace dust::render::vulkan
