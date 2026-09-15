/**
 * Generic lazy segment tree.
 *
 * Policy M must define:
 *   using S, using F
 *   static S op(S, S), e()
 *   static S mapping(F, S)
 *   static F composition(F f, F g), id()
 *
 * composition(f, g) means applying g first, then f.
 * Every range is represented as [l, r).
 * Time: O(log N) per set, get, query, and apply.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

template<class M>
struct LazySegmentTree {
	using S = typename M::S;
	using F = typename M::F;

	int n = 0, size = 1, log = 0;
	std::vector<S> data;
	std::vector<F> lazy;

	LazySegmentTree() : LazySegmentTree(0) {}
	explicit LazySegmentTree(int n) : LazySegmentTree(std::vector<S>(n, M::e())) {}
	explicit LazySegmentTree(const std::vector<S>& values) : n((int)values.size()) {
		while (size < n) size *= 2, ++log;
		data.assign(2 * size, M::e());
		lazy.assign(size, M::id());
		std::copy(values.begin(), values.end(), data.begin() + size);
		for (int k = size - 1; k; --k) pull(k);
	}

	void set(int p, const S& value) {
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		data[p] = value;
		for (int i = 1; i <= log; ++i) pull(p >> i);
	}

	S get(int p) {
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		return data[p];
	}

	S query(int l, int r) {
		assert(0 <= l && l <= r && r <= n);
		if (l == r) return M::e();
		l += size, r += size;
		for (int i = log; i; --i) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		S left = M::e(), right = M::e();
		while (l < r) {
			if (l & 1) left = M::op(left, data[l++]);
			if (r & 1) right = M::op(data[--r], right);
			l /= 2, r /= 2;
		}
		return M::op(left, right);
	}

	S all_query() const { return data[1]; }

	void apply(int p, const F& operation) {
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		data[p] = M::mapping(operation, data[p]);
		for (int i = 1; i <= log; ++i) pull(p >> i);
	}

	void apply(int l, int r, const F& operation) {
		assert(0 <= l && l <= r && r <= n);
		if (l == r) return;
		l += size, r += size;
		for (int i = log; i; --i) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		int original_l = l, original_r = r;
		while (l < r) {
			if (l & 1) all_apply(l++, operation);
			if (r & 1) all_apply(--r, operation);
			l /= 2, r /= 2;
		}
		l = original_l, r = original_r;
		for (int i = 1; i <= log; ++i) {
			if (((l >> i) << i) != l) pull(l >> i);
			if (((r >> i) << i) != r) pull((r - 1) >> i);
		}
	}

private:
	void pull(int k) { data[k] = M::op(data[2 * k], data[2 * k + 1]); }

	void all_apply(int k, const F& operation) {
		data[k] = M::mapping(operation, data[k]);
		if (k < size) lazy[k] = M::composition(operation, lazy[k]);
	}

	void push(int k) {
		all_apply(2 * k, lazy[k]);
		all_apply(2 * k + 1, lazy[k]);
		lazy[k] = M::id();
	}
};

template<class M>
using GenericLazySegmentTree = LazySegmentTree<M>;
