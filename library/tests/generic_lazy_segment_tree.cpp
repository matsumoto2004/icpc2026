#include "library/data_structure/generic_lazy_segment_tree.hpp"

#include <iostream>
#include <limits>
#include <random>
#include <string>

using ll = long long;

// The header must not reserve the name M for an example policy.
struct M {
	using S = ll;
	using F = ll;
	static S op(S a, S b) { return std::min(a, b); }
	static S e() { return std::numeric_limits<S>::max() / 4; }
	static S mapping(F f, S x) { return x == e() ? x : x + f; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};

struct AffineSum {
	static constexpr ll mod = 998244353;
	struct S { ll sum; int len; };
	struct F { ll a, b; };
	static S op(S a, S b) { return {(a.sum + b.sum) % mod, a.len + b.len}; }
	static S e() { return {0, 0}; }
	static S mapping(F f, S x) { return {(f.a * x.sum + f.b * x.len) % mod, x.len}; }
	static F composition(F f, F g) {
		return {f.a * g.a % mod, (f.a * g.b + f.b) % mod};
	}
	static F id() { return {1, 0}; }
};

struct SumAdd {
	struct S { ll sum; int len; };
	using F = ll;
	static S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }
	static S e() { return {0, 0}; }
	static S mapping(F f, S x) { return {x.sum + f * x.len, x.len}; }
	static F composition(F f, F g) { return f + g; }
	static F id() { return 0; }
};

struct Concat {
	using S = std::string;
	using F = bool;
	static S op(S a, S b) { return a + b; }
	static S e() { return {}; }
	static S mapping(F f, S x) { return f ? S{} : x; }
	static F composition(F f, F g) { return f || g; }
	static F id() { return false; }
};

void check_min_add(std::mt19937& rng) {
	for (int test = 0; test < 150; ++test) {
		int n = 1 + rng() % 100;
		int origin = (int)(rng() % 101) - 50;
		std::vector<ll> values(n);
		LazySegmentTree<M> seg(origin, origin + n);
		for (int i = 0; i < n; ++i) {
			values[i] = (int)(rng() % 201) - 100;
			assert(seg.get(origin + i) == M::e());
			seg.set(origin + i, values[i]);
		}
		for (int query = 0; query < 400; ++query) {
			int l = rng() % (n + 1), r = rng() % (n + 1);
			if (l > r) std::swap(l, r);
			ll value = (int)(rng() % 21) - 10;
			switch (rng() % 5) {
			case 0:
				seg.apply(origin + l, origin + r, value);
				for (int i = l; i < r; ++i) values[i] += value;
				break;
			case 1:
				seg.add(origin + l, origin + r, value);
				for (int i = l; i < r; ++i) values[i] += value;
				break;
			case 2: {
				int p = rng() % n;
				seg.set(origin + p, value);
				values[p] = value;
				break;
			}
			case 3: {
				int p = rng() % n;
				seg.apply(origin + p, value);
				values[p] += value;
				break;
			}
			default: {
				ll expected = M::e();
				for (int i = l; i < r; ++i) expected = std::min(expected, values[i]);
				assert(seg.query(origin + l, origin + r) == expected);
			}
			}
			assert(seg.all_query() == *std::min_element(values.begin(), values.end()));
			int from = rng() % (n + 1), expected = from;
			ll limit = (int)(rng() % 201) - 100;
			while (expected < n && values[expected] >= limit) ++expected;
			assert(seg.max_right(origin + from, [&](ll x) { return x >= limit; }) == origin + expected);
		}
		for (int i = 0; i < n; ++i) assert(seg.get(origin + i) == values[i]);
		LazySegmentTree<M> from_values(values);
		assert(from_values.query(0, n) == seg.all_query());
	}
}

void check_affine_sum(std::mt19937& rng) {
	for (int test = 0; test < 100; ++test) {
		int n = 1 + rng() % 100;
		std::vector<ll> values(n);
		std::vector<AffineSum::S> initial;
		for (ll& value : values) {
			value = rng() % 1000;
			initial.push_back({value, 1});
		}
		LazySegmentTree<AffineSum> seg(initial);
		for (int query = 0; query < 400; ++query) {
			int l = rng() % (n + 1), r = rng() % (n + 1);
			if (l > r) std::swap(l, r);
			if (rng() % 2) {
				AffineSum::F f{rng() % 4, rng() % 10};
				seg.apply(l, r, f);
				for (int i = l; i < r; ++i)
					values[i] = (f.a * values[i] + f.b) % AffineSum::mod;
			} else {
				ll expected = 0;
				for (int i = l; i < r; ++i) expected = (expected + values[i]) % AffineSum::mod;
				auto actual = seg.query(l, r);
				assert(actual.sum == expected && actual.len == r - l);
			}
		}
		for (int i = 0; i < n; ++i) assert(seg.get(i).sum == values[i]);
	}
}

void check_max_right(std::mt19937& rng) {
	for (int test = 0; test < 150; ++test) {
		int n = 1 + rng() % 80, origin = (int)(rng() % 101) - 50;
		std::vector<ll> a(n);
		LazySegmentTree<SumAdd> seg(origin, origin + n);
		for (int i = 0; i < n; ++i) {
			a[i] = rng() % 5;
			seg.set(origin + i, {a[i], 1});
		}
		for (int query = 0; query < 300; ++query) {
			if (rng() % 2) {
				int l = rng() % (n + 1), r = rng() % (n + 1);
				if (l > r) std::swap(l, r);
				ll f = rng() % 4;
				seg.apply(origin + l, origin + r, f);
				for (int i = l; i < r; ++i) a[i] += f;
			} else {
				int p = rng() % n;
				a[p] = rng() % 5;
				seg.set(origin + p, {a[p], 1});
			}
			int l = rng() % (n + 1), expected = l;
			ll total = 0, sum = 0;
			for (int i = l; i < n; ++i) total += a[i];
			ll k = 1 + rng() % (total + 4);
			while (expected < n && sum + a[expected] < k) sum += a[expected++];
			assert(seg.max_right(origin + l, [&](SumAdd::S x) { return x.sum < k; }) == origin + expected);
			assert(seg.max_right(origin + l, [](SumAdd::S) { return true; }) == origin + n);
		}
	}
	LazySegmentTree<SumAdd> slots(std::vector<SumAdd::S>(5, {1, 1}));
	slots.set(1, {0, 1});
	slots.set(3, {0, 1});
	for (int k = 1; k <= 3; ++k)
		assert(slots.max_right(0, [&](SumAdd::S x) { return x.sum < k; }) == 2 * (k - 1));
	LazySegmentTree<Concat> strings(std::vector<std::string>{"a", "b", "c", "d", "e"});
	std::string prefix = "bcdz";
	assert(strings.max_right(1, [&](const std::string& x) { return prefix.starts_with(x); }) == 4);
	strings.apply(1, 4, true);
	assert(strings.max_right(0, [](const std::string& x) { return std::string("aez").starts_with(x); }) == 5);
}

int main() {
	LazySegmentTree<M> empty;
	assert(empty.query(0, 0) == M::e() && empty.all_query() == M::e());
	empty.add(0, 0, 10);
	LazySegmentTree<M> shifted_empty(7, 7);
	assert(shifted_empty.query(7, 7) == M::e());
	shifted_empty.add(7, 7, 10);
	assert(empty.max_right(0, [](ll) { return true; }) == 0);
	assert(shifted_empty.max_right(7, [](ll) { return true; }) == 7);
	std::mt19937 rng(20231025);
	check_min_add(rng);
	check_affine_sum(rng);
	check_max_right(rng);
	std::cout << "Generic lazy segment tree tests passed\n";
}
