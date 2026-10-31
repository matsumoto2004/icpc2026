// Source: https://github.com/matsumoto2004/icpc2026
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
 * Construct with values, a size N (coordinates [0, N)), or bounds (lo, hi).
 * add(l, r, f) is an alias for apply(l, r, f), for KACTL HLD compatibility.
 * max_right(l, g): largest r with g(query(l, r)) == true, in original coordinates.
 * Requires g(e()) == true and g to change from true to false as r increases.
 * Time: O(log N) per set, get, query, apply, and max_right.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

template<class M>
struct LazySegmentTree {
	using S = typename M::S;
	using F = typename M::F;

	int n = 0, size = 1, log = 0, origin = 0;
	std::vector<S> data;
	std::vector<F> lazy;

	LazySegmentTree() : LazySegmentTree(0) {}
	explicit LazySegmentTree(int n) : LazySegmentTree(std::vector<S>(n, M::e())) {}
	LazySegmentTree(int lo, int hi) : LazySegmentTree(len(lo, hi)) {
		origin = lo;
	}
	explicit LazySegmentTree(const std::vector<S>& v) : n((int)v.size()) {
		while (size < n) size *= 2, ++log;
		data.assign(2 * size, M::e());
		lazy.assign(size, M::id());
		std::copy(v.begin(), v.end(), data.begin() + size);
		for (int k = size - 1; k; --k) pull(k);
	}

	void set(int p, const S& x) {
		p -= origin;
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		data[p] = x;
		for (int i = 1; i <= log; ++i) pull(p >> i);
	}

	S get(int p) {
		p -= origin;
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		return data[p];
	}

	S query(int l, int r) {
		l -= origin, r -= origin;
		assert(0 <= l && l <= r && r <= n);
		if (l == r) return M::e();
		l += size, r += size;
		for (int i = log; i; --i) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		S a = M::e(), b = M::e();
		while (l < r) {
			if (l & 1) a = M::op(a, data[l++]);
			if (r & 1) b = M::op(data[--r], b);
			l /= 2, r /= 2;
		}
		return M::op(a, b);
	}

	S all_query() const { return data[1]; }

	// Adapted from ACL's max_right (CC0).
	// https://github.com/atcoder/ac-library/blob/master/atcoder/lazysegtree.hpp
	template<class G>
	int max_right(int l, G g) {
		l -= origin;
		assert(0 <= l && l <= n && g(M::e()));
		if (l == n) return origin + n;
		l += size;
		for (int i = log; i; --i) push(l >> i);
		S x = M::e();
		do {
			while (l % 2 == 0) l /= 2;
			S y = M::op(x, data[l]);
			if (!g(y)) {
				while (l < size) {
					push(l);
					l *= 2;
					y = M::op(x, data[l]);
					if (g(y)) x = y, ++l;
				}
				return origin + l - size;
			}
			x = y;
			++l;
		} while ((l & -l) != l);
		return origin + n;
	}

	void apply(int p, const F& f) {
		p -= origin;
		assert(0 <= p && p < n);
		p += size;
		for (int i = log; i; --i) push(p >> i);
		data[p] = M::mapping(f, data[p]);
		for (int i = 1; i <= log; ++i) pull(p >> i);
	}

	void apply(int l, int r, const F& f) {
		l -= origin, r -= origin;
		assert(0 <= l && l <= r && r <= n);
		if (l == r) return;
		l += size, r += size;
		for (int i = log; i; --i) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		int a = l, b = r;
		while (l < r) {
			if (l & 1) put(l++, f);
			if (r & 1) put(--r, f);
			l /= 2, r /= 2;
		}
		l = a, r = b;
		for (int i = 1; i <= log; ++i) {
			if (((l >> i) << i) != l) pull(l >> i);
			if (((r >> i) << i) != r) pull((r - 1) >> i);
		}
	}

	//for HLD
	void add(int l, int r, const F& f) { apply(l, r, f); }

private:
	static int len(int lo, int hi) {
		assert(lo <= hi);
		return hi - lo;
	}

	void pull(int k) { data[k] = M::op(data[2 * k], data[2 * k + 1]); }

	void put(int k, const F& f) {
		data[k] = M::mapping(f, data[k]);
		if (k < size) lazy[k] = M::composition(f, lazy[k]);
	}

	void push(int k) {
		put(2 * k, lazy[k]);
		put(2 * k + 1, lazy[k]);
		lazy[k] = M::id();
	}
};

/* Mの例1：区間加算・区間最小値。必要なMをコメント外にコピーする。
struct M {
	using S = int; // 区間の最小値
	using F = int; // 加算量
	static S op(S a, S b) { return std::min(a, b); }
	static S e() { return 1 << 30; }
	static S mapping(F f, S x) { return x == e() ? x : x + f; }
	static F composition(F f, F g) { return f + g; } // gの後にf
	static F id() { return 0; }
};

// main内：初期値0のn要素。個数だけで構築するとe()になるので配列を渡す。
LazySegmentTree<M> seg(std::vector<int>(n, 0));
*/

/* Mの例2：区間加算・区間総和。例1とは別のMとして使う。
struct M {
	struct S { long long sum; int len; }; // 総和・区間長
	using F = long long; // 加算量
	static S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }
	static S e() { return {0, 0}; }
	static S mapping(F f, S x) { return {x.sum + f * x.len, x.len}; }
	static F composition(F f, F g) { return f + g; } // gの後にf
	static F id() { return 0; }
};

// main内：初期値0のn要素。各要素のlenは1にする。
LazySegmentTree<M> seg(std::vector<M::S>(n, {0, 1}));

// 左からk番目の空き（1 <= k <= 空き総数）。空き=1、使用済み=0。
LazySegmentTree<M> st(std::vector<M::S>(n, {1, 1}));
int p = st.max_right(0, [&](M::S x) { return x.sum < k; });
st.set(p, {0, 1}); // pを使用済みにする
*/
