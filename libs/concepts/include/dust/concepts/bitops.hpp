#pragma once

#include <type_traits>

namespace dust
{

template<typename T>
concept EnumClass = std::is_enum_v<T> and not std::is_convertible_v<T, std::underlying_type_t<T>>;

template<EnumClass T>
inline constexpr bool enableEnumBitops = false;

template<typename T>
concept EnumBitops = EnumClass<T> and enableEnumBitops<T>;

template<EnumBitops T>
[[nodiscard]] constexpr T operator|(T a, T b) noexcept
{
	using I = std::underlying_type_t<T>;

	return static_cast<T>(static_cast<I>(a) | static_cast<I>(b));
}

template<EnumBitops T>
constexpr T &operator|=(T &a, T b) noexcept
{
	return a = a | b;
}

template<EnumBitops T>
[[nodiscard]] constexpr T operator&(T a, T b) noexcept
{
	using I = std::underlying_type_t<T>;

	return static_cast<T>(static_cast<I>(a) & static_cast<I>(b));
}

} // namespace dust