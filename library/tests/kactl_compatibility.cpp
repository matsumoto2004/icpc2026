#include <bits/stdc++.h>
using namespace std;
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;

#include "kactl/content/number-theory/ModularArithmetic.h"
#include "kactl/content/data-structures/Matrix.h"
#include "kactl/content/data-structures/FenwickTree.h"
#include "kactl/content/data-structures/UnionFindRollback.h"
#include "kactl/content/geometry/Point.h"
#include "kactl/content/graph/LCA.h"
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
	using S = Mod;
	using F = Mod;
	static S op(S a, S b) { return a + b; }
	static S e() { return 0; }
	static S mapping(F f, S x) { return f * x; }
	static F composition(F f, F g) { return f * g; }
	static F id() { return 1; }
};

struct TreePolicy {
	using DP = Mod;
	using Edge = int;
	DP identity() const { return 0; }
	DP merge(DP a, DP b) const { return a + b; }
	DP add_edge(DP x, const Edge&, int, int) const { return x; }
	DP add_vertex(DP x, int) const { return x + Mod(1); }
};

int main() {
	check_combination<Mod>(mod - 1, mod);
	check_combination<ModInt<17>>(16, 17);
	check_combination<ModInt<998244353>>(200, 998244353);
	using Mint = ModInt<17>;
	for (int a = 1; a < 17; ++a) {
		assert(Mint(0).invert(a).x == Mod(0).invert(a).x);
		assert((Mint(a) ^ 13).x == (Mod(a) ^ 13).x);
	}
	Matrix<Mint, 2> matrix;
	matrix.d = {{{1, 1}, {1, 0}}};
	assert((matrix ^ 10).d[0][1].x == 55 % 17);
	LazySegmentTree<SumPolicy> seg(vector<Mod>{1, 2, 3});
	seg.apply(0, 2, Mod(3));
	assert(seg.query(0, 3).x == 12);
	Rerooting<TreePolicy> tree(3);
	tree.add_edge(0, 1);
	tree.add_edge(1, 2);
	for (Mod value : tree.solve()) assert(value.x == 3);
	vector<vi> graph{{1}, {0, 2}, {1}};
	EulerTour tour(graph);
	LCA lca(graph);
	assert(tour.subtree_size(1) == 2 && lca.lca(1, 2) == 1);
	FT fenwick(3);
	fenwick.update(tour.in[2], 4);
	assert(fenwick.query(tour.out[1]) - fenwick.query(tour.in[1]) == 4);
	vector<Point<ll>> points{Point<ll>(2, 0), Point<ll>(1, 0), Point<ll>(1, 0)};
	Compress<Point<ll>> compressed(points);
	assert(compressed.size() == 2 && inversion_count(points) == 2);
	ostringstream output;
	auto* previous = cerr.rdbuf(output.rdbuf());
	vector<pair<Mod, vector<Mod>>> values{{Mod(3), {Mod(4), Mod(5)}}};
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
