#pragma once

#include <algorithm>
#include <set>
#include <vector>

namespace dust::render::vulkan
{

template<class CollectionT, class KeyT>
auto makeSet(
	const CollectionT &collection, KeyT key = [](auto &&value) { return value; })
{
	auto _set = std::set<std::invoke_result_t<KeyT, typename CollectionT::value_type>> {};

	std::transform(collection.begin(), collection.end(), std::inserter(_set, _set.end()), key);

	return _set;
}

template<class AvailableT, class RequiredT, class KeyT>
auto getMissing(const AvailableT &available, const RequiredT &required, KeyT key)
{
	const auto availableSet = makeSet(available, key);
	auto missing = std::vector<typename RequiredT::value_type> {};

	for (const auto &value : required)
	{
		if (not availableSet.contains(value))
		{
			missing.push_back(value);
		}
	}

	return missing;
}

} // namespace dust::render::vulkan
