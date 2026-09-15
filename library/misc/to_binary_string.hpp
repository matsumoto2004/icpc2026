/**
 * Converts a non-negative integer to base 2.
 * min_width pads the result with leading zeroes.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <string>
#include <type_traits>

template<std::integral T>
requires (!std::same_as<std::remove_cv_t<T>, bool>)
std::string to_binary_string(T value, int min_width = 0) {
	assert(min_width >= 0);
	if constexpr (std::signed_integral<T>) assert(value >= 0);
	using U = std::make_unsigned_t<T>;
	U x = (U)value;
	std::string result;
	do {
		result.push_back(char('0' + x % 2));
		x /= 2;
	} while (x);
	while ((int)result.size() < min_width) result.push_back('0');
	std::reverse(result.begin(), result.end());
	return result;
}
