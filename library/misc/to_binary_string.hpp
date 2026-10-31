// Source: https://github.com/matsumoto2004/icpc2026
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
std::string to_binary_string(T v, int w = 0) {
	assert(w >= 0);
	if constexpr (std::signed_integral<T>) assert(v >= 0);
	using U = std::make_unsigned_t<T>;
	U x = (U)v;
	std::string s;
	do {
		s.push_back(char('0' + x % 2));
		x /= 2;
	} while (x);
	while ((int)s.size() < w) s.push_back('0');
	std::reverse(s.begin(), s.end());
	return s;
}
