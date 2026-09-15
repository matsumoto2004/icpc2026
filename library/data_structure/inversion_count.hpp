/**
 * Counts pairs (i, j) such that i < j and values[i] > values[j].
 * Duplicate values are not counted as inversions.
 * Time: O(N log N), Memory: O(N).
 */
#pragma once

#include <vector>

#include "coordinate_compression.hpp"

namespace inversion_count_detail {

struct FenwickTree {
	std::vector<int> data;

	explicit FenwickTree(int n) : data(n + 1) {}

	void add(int p) {
		for (++p; p < (int)data.size(); p += p & -p) ++data[p];
	}

	int sum(int r) const {
		int result = 0;
		for (; r > 0; r -= r & -r) result += data[r];
		return result;
	}
};

} // namespace inversion_count_detail

template<class T>
long long inversion_count(const std::vector<T>& values) {
	Compress<T> compressed(values);
	inversion_count_detail::FenwickTree tree(compressed.size());
	long long result = 0;
	for (int i = 0; i < (int)values.size(); ++i) {
		int rank = compressed.rank(values[i]);
		result += i - tree.sum(rank + 1);
		tree.add(rank);
	}
	return result;
}
