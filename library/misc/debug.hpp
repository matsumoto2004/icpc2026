#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace debug_detail {

template<class A, class B>
void print(const std::pair<A, B>& value);

template<class T>
void print(const std::vector<T>& values);

template<class T>
void print(const T& value) {
	if constexpr (requires { std::cerr << value; }) {
		std::cerr << value;
	} else if constexpr (requires { value.x; }) {
		// KACTL's Mod exposes x but has no stream insertion operator.
		static_assert(std::is_arithmetic_v<decltype(value.x)>);
		std::cerr << value.x;
	} else {
		static_assert(sizeof(T) == 0, "Unsupported debug value type");
	}
}

template<class A, class B>
void print(const std::pair<A, B>& value) {
	std::cerr << '(';
	print(value.first);
	std::cerr << ", ";
	print(value.second);
	std::cerr << ')';
}

template<class T>
void print(const std::vector<T>& values) {
	std::cerr << '[';
	for (int i = 0; i < (int)values.size(); ++i) {
		if (i) std::cerr << ", ";
		print(values[i]);
	}
	std::cerr << ']';
}

} // namespace debug_detail

#ifdef LOCAL
#define debug(x) \
	(std::cerr << #x << " = ", debug_detail::print(x), std::cerr << '\n')
#else
#define debug(x) ((void)0)
#endif
