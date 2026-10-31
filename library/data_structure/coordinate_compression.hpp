// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Description: Coordinate compression shared by multiple inputs.
 * Add every value before build(). rank(x) returns the zero-based compressed
 * value, and value(i) or operator[] restores the original value.
 *
 * Usage:
 * Compress<long long> comp;
 * comp.add(a);
 * comp.add(b);
 * comp.add(x);
 * comp.build();
 * vector<int> ca = comp.compress(a);
 * long long original = comp[ca[0]];
 *
 * Time: O(N log N) to build, O(log N) per rank lookup.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <initializer_list>
#include <iterator>
// KACTL's all(x) macro otherwise expands std::views::all in libc++ headers.
#pragma push_macro("all")
#undef all
#include <ranges>
#pragma pop_macro("all")
#include <vector>

template<class T>
struct Compress {
	Compress() = default;

	explicit Compress(const std::vector<T>& a) { add(a); build(); }

	void add(const T& x) { assert(!ok); v.push_back(x); }

	template<class It>
	void add(It l, It r) {
		assert(!ok);
		v.insert(v.end(), l, r);
	}

	template<std::ranges::input_range R>
	requires std::convertible_to<std::ranges::range_reference_t<R>, T>
	void add(const R& a) {
		add(std::ranges::begin(a), std::ranges::end(a));
	}

	void add(std::initializer_list<T> a) { add(a.begin(), a.end()); }

	void build() {
		assert(!ok);
		std::sort(v.begin(), v.end());
		v.erase(std::unique(v.begin(), v.end()), v.end());
		ok = true;
	}

	int size() const { assert(ok); return (int)v.size(); }

	bool empty() const { assert(ok); return v.empty(); }

	// Number of registered values strictly smaller than x.
	int lower_rank(const T& x) const {
		assert(ok);
		return (int)(std::lower_bound(v.begin(), v.end(), x) - v.begin());
	}

	// Number of registered values smaller than or equal to x.
	int upper_rank(const T& x) const {
		assert(ok);
		return (int)(std::upper_bound(v.begin(), v.end(), x) - v.begin());
	}

	bool contains(const T& x) const {
		assert(ok);
		int i = lower_rank(x);
		return i < size() && !(x < v[i]);
	}

	// Exact rank of a value registered before build().
	int rank(const T& x) const {
		int i = lower_rank(x);
		assert(i < size() && !(x < v[i]));
		return i;
	}

	const T& value(int b) const {
		assert(ok);
		assert(0 <= b && b < (int)v.size());
		return v[b];
	}

	const T& operator[](int b) const { return value(b); }

	template<std::ranges::input_range R>
	requires std::convertible_to<std::ranges::range_reference_t<R>, T>
	std::vector<int> compress(const R& a) const {
		assert(ok);
		std::vector<int> res;
		for (auto it = std::ranges::begin(a); it != std::ranges::end(a); ++it)
			res.push_back(rank(*it));
		return res;
	}

	std::vector<T> restore(const std::vector<int>& b) const {
		assert(ok);
		std::vector<T> res;
		res.reserve(b.size());
		for (int i : b) res.push_back(value(i));
		return res;
	}

	const std::vector<T>& values() const { assert(ok); return v; }

private:
	std::vector<T> v;
	bool ok = false;
};
