#pragma once

#include <cstdint>

#include <dust/concepts/bitops.hpp>

namespace dust
{

namespace storage
{

enum class StreamFlags : uint32_t
{
	Default = 0x00000000,
	Truncate = 0x00000001
};

} // namespace storage

template<>
inline constexpr bool enableEnumBitops<dust::storage::StreamFlags> = true;

} // namespace dust