// Source: https://github.com/matsumoto2004/icpc2026
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
		int res = 0;
		for (; r > 0; r -= r & -r) res += data[r];
		return res;
	}
};

} // namespace inversion_count_detail

template<class T>
long long inversion_count(const std::vector<T>& a) {
	Compress<T> c(a);
	inversion_count_detail::FenwickTree ft(c.size());
	long long res = 0;
	for (int i = 0; i < (int)a.size(); ++i) {
		int p = c.rank(a[i]);
		res += i - ft.sum(p + 1);
		ft.add(p);
	}
	return res;
}
