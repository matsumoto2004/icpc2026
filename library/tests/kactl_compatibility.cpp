#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;

namespace kactl_mod {
#include "kactl/content/number-theory/ModularArithmetic.h"
}
using KMod = kactl_mod::Mod;
#include "kactl/content/data-structures/Matrix.h"
#include "kactl/content/data-structures/FenwickTree.h"
#include "kactl/content/data-structures/UnionFindRollback.h"
#include "kactl/content/geometry/Point.h"
#include "kactl/content/graph/LCA.h"
#include "library/math/modint_manyfunc.hpp"
#include "library/math/combination.hpp"
#include "library/data_structure/inversion_count.hpp"
#include "library/data_structure/generic_lazy_segment_tree.hpp"
#include "library/graph/euler_tour.hpp"
#include "library/graph/rerooting.hpp"
#include "library/misc/random.hpp"
#include "library/misc/to_binary_string.hpp"
#include "library/misc/debug.hpp"

template<class Mint>
void check_combination(int limit, int modulus) {
	vector<ll> row(limit + 1);
	row[0] = 1;
	for (int n = 0; n <= limit; ++n) {
		const Combination<Mint> comb(n);
		for (int r = 0; r <= n; ++r) assert(comb.C(n, r).x == row[r]);
		assert(comb.C(-1, 0).x == 0);
		assert(comb.C(n, -1).x == 0);
		assert(comb.C(n, n + 1).x == 0);
		for (int r = n + 1; r > 0 && r <= limit; --r)
			row[r] = (row[r] + row[r - 1]) % modulus;
	}
}

struct SumPolicy {
	using S = KMod;
	using F = KMod;
	static S op(S a, S b) { return a + b; }
	static S e() { return 0; }
	static S mapping(F f, S x) { return f * x; }
	static F composition(F f, F g) { return f * g; }
	static F id() { return 1; }
};

struct TreePolicy {
	using DP = KMod;
	using Edge = int;
	DP identity() const { return 0; }
	DP merge(DP a, DP b) const { return a + b; }
	DP add_edge(DP x, const Edge&, int, int) const { return x; }
	DP add_vertex(DP x, int) const { return x + KMod(1); }
};

struct DistancePolicy {
	struct DP { ll sum, count; };
	using Edge = int;
	DP identity() const { return {0, 0}; }
	DP merge(DP a, DP b) const { return {a.sum + b.sum, a.count + b.count}; }
	DP add_edge(DP a, const Edge& w, int, int) const {
		return {a.sum + a.count * w, a.count};
	}
	DP add_vertex(DP a, int) const { return {a.sum, a.count + 1}; }
};

int main() {
	check_combination<KMod>(kactl_mod::mod - 1, kactl_mod::mod);
	check_combination<ModInt<17>>(16, 17);
	check_combination<ModInt<998244353>>(200, 998244353);
	using Mint = ModInt<17>;
	assert(Mint(20).value() == 3 && Mint::mod() == 17);
	assert(+Mint(3) == Mint(3) && -Mint(3) == Mint(14));
	istringstream input("-1");
	Mint parsed;
	input >> parsed;
	ostringstream formatted;
	formatted << parsed;
	assert(formatted.str() == "16");
	for (int a = 1; a < 17; ++a) {
		assert(Mint(0).invert(a).x == KMod(0).invert(a).x);
		assert((Mint(a) ^ 13).x == (KMod(a) ^ 13).x);
	}
	Matrix<Mint, 2> matrix;
	matrix.d = {{{1, 1}, {1, 0}}};
	assert((matrix ^ 10).d[0][1].x == 55 % 17);
	LazySegmentTree<SumPolicy> seg(vector<KMod>{1, 2, 3});
	seg.apply(0, 2, KMod(3));
	assert(seg.query(0, 3).x == 12);
	Rerooting<TreePolicy> tree(3);
	tree.add_edge(0, 1);
	tree.add_edge(1, 2);
	for (KMod value : tree.solve()) assert(value.x == 3);
	Rerooting<DistancePolicy> distances(4);
	distances.add_edge(0, 1, 3);
	distances.add_edge(1, 2, 5);
	distances.add_edge(1, 3, 7);
	auto sums = distances.solve(2);
	vector<ll> expected{21, 15, 25, 29};
	for (int i = 0; i < 4; ++i)
		assert(sums[i].sum == expected[i] && sums[i].count == 4);
	vector<vi> graph{{1}, {0, 2}, {1}};
	EulerTour tour(graph);
	LCA lca(graph);
	assert(tour.subtree_size(1) == 2 && lca.lca(1, 2) == 1);
	EulerTour rerooted(graph, 1);
	assert(rerooted.parent[1] == 1 && rerooted.depth[0] == 1);
	assert(rerooted.subtree_range(1) == make_pair(0, 3));
	FT fenwick(3);
	fenwick.update(tour.in[2], 4);
	assert(fenwick.query(tour.out[1]) - fenwick.query(tour.in[1]) == 4);
	vector<Point<ll>> points{Point<ll>(2, 0), Point<ll>(1, 0), Point<ll>(1, 0)};
	Compress<Point<ll>> compressed(points);
	assert(compressed.size() == 2 && inversion_count(points) == 2);
	Compress<int> comp;
	vector<int> a{5, -2, 5};
	comp.add(a);
	comp.add({7, -2});
	comp.build();
	assert(!comp.empty() && comp.values() == vector<int>({-2, 5, 7}));
	assert(comp.compress(a) == vector<int>({1, 0, 1}));
	assert(comp.restore(comp.compress(a)) == a && comp[1] == 5);
	assert(comp.lower_rank(4) == 1 && comp.upper_rank(5) == 2);
	assert(comp.contains(5) && !comp.contains(6));
	assert(inversion_count(vector<int>{2, 1, 2, 0}) == 4);
	assert(to_binary_string(0) == "0" && to_binary_string(5, 5) == "00101");
	assert(to_binary_string(ULLONG_MAX) == string(64, '1'));
	Random ra(123), rb(123);
	for (int i = 0; i < 100; ++i) {
		assert(ra.next_u64() == rb.next_u64());
		int x = ra.range(-3, 4);
		assert(x == rb.range(-3, 4) && -3 <= x && x < 4);
		ll y = ra.inclusive(-100LL, 100LL);
		assert(y == rb.inclusive(-100LL, 100LL) && -100 <= y && y <= 100);
		assert(ra.range(10) == rb.range(10));
		assert(ra.range(10LL) == rb.range(10LL));
		assert(ra.range(-3LL, 4LL) == rb.range(-3LL, 4LL));
		assert(ra.inclusive(-3, 4) == rb.inclusive(-3, 4));
		assert(ra.boolean() == rb.boolean() && ra.real() == rb.real());
	}
	vector<int> shuffled{1, 2, 3, 4}, shuffled_copy = shuffled;
	ra.shuffle(shuffled.begin(), shuffled.end());
	rb.shuffle(shuffled_copy.begin(), shuffled_copy.end());
	assert(shuffled == shuffled_copy);
	ostringstream output;
	auto* previous = cerr.rdbuf(output.rdbuf());
	vector<pair<KMod, vector<KMod>>> values{{KMod(3), {KMod(4), KMod(5)}}};
	debug_detail::print(values);
	assert(output.str() == "[(3, [4, 5])]");
	output.str("");
	debug(values);
#ifdef LOCAL
	assert(output.str() == "values = [(3, [4, 5])]\n");
#else
	assert(output.str().empty());
#endif
	cerr.rdbuf(previous);
	cout << "KACTL compatibility tests passed\n";
}
