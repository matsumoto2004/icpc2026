// Source: https://github.com/matsumoto2004/icpc2026
#pragma once

#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace debug_detail {

template<class A, class B>
void print(const std::pair<A, B>& a);

template<class T>
void print(const std::vector<T>& v);

template<class T>
void print(const T& a) {
	if constexpr (requires { std::cerr << a; }) {
		std::cerr << a;
	} else if constexpr (requires { a.x; }) {
		// KACTL's Mod exposes x but has no stream insertion operator.
		static_assert(std::is_arithmetic_v<decltype(a.x)>);
		std::cerr << a.x;
	} else {
		static_assert(sizeof(T) == 0, "Unsupported debug value type");
	}
}

template<class A, class B>
void print(const std::pair<A, B>& a) {
	std::cerr << '(';
	print(a.first);
	std::cerr << ", ";
	print(a.second);
	std::cerr << ')';
}

template<class T>
void print(const std::vector<T>& v) {
	std::cerr << '[';
	for (int i = 0; i < (int)v.size(); ++i) {
		if (i) std::cerr << ", ";
		print(v[i]);
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
