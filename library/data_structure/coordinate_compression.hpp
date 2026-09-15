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

	explicit Compress(const std::vector<T>& input) {
		add(input);
		build();
	}

	void add(const T& x) {
		assert(!built_);
		values_.push_back(x);
	}

	template<class Iterator>
	void add(Iterator first, Iterator last) {
		assert(!built_);
		values_.insert(values_.end(), first, last);
	}

	template<std::ranges::input_range Range>
	requires std::convertible_to<std::ranges::range_reference_t<Range>, T>
	void add(const Range& input) {
		add(std::ranges::begin(input), std::ranges::end(input));
	}

	void add(std::initializer_list<T> input) {
		add(input.begin(), input.end());
	}

	void build() {
		assert(!built_);
		std::sort(values_.begin(), values_.end());
		values_.erase(std::unique(values_.begin(), values_.end()), values_.end());
		built_ = true;
	}

	int size() const {
		assert(built_);
		return (int)values_.size();
	}

	bool empty() const {
		assert(built_);
		return values_.empty();
	}

	// Number of registered values strictly smaller than x.
	int lower_rank(const T& x) const {
		assert(built_);
		return (int)(std::lower_bound(values_.begin(), values_.end(), x) - values_.begin());
	}

	// Number of registered values smaller than or equal to x.
	int upper_rank(const T& x) const {
		assert(built_);
		return (int)(std::upper_bound(values_.begin(), values_.end(), x) - values_.begin());
	}

	bool contains(const T& x) const {
		assert(built_);
		int i = lower_rank(x);
		return i < size() && !(x < values_[i]);
	}

	// Exact rank of a value registered before build().
	int rank(const T& x) const {
		int i = lower_rank(x);
		assert(i < size() && !(x < values_[i]));
		return i;
	}

	const T& value(int compressed) const {
		assert(built_);
		assert(0 <= compressed && compressed < (int)values_.size());
		return values_[compressed];
	}

	const T& operator[](int compressed) const {
		return value(compressed);
	}

	template<std::ranges::input_range Range>
	requires std::convertible_to<std::ranges::range_reference_t<Range>, T>
	std::vector<int> compress(const Range& input) const {
		assert(built_);
		std::vector<int> result;
		for (auto it = std::ranges::begin(input); it != std::ranges::end(input); ++it)
			result.push_back(rank(*it));
		return result;
	}

	std::vector<T> restore(const std::vector<int>& compressed) const {
		assert(built_);
		std::vector<T> result;
		result.reserve(compressed.size());
		for (int i : compressed) result.push_back(value(i));
		return result;
	}

	const std::vector<T>& values() const {
		assert(built_);
		return values_;
	}

private:
	std::vector<T> values_;
	bool built_ = false;
};
