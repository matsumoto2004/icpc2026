// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Generic rerooting DP on a tree.
 *
 * Policy M must define:
 *   using DP, using Edge
 *   DP identity() const
 *   DP merge(DP left, DP right) const
 *   DP add_edge(DP value, const Edge& edge, int from, int to) const
 *   DP add_vertex(DP value, int vertex) const
 *
 * merge must be associative and identity() must be its identity element.
 * add_edge converts a DP value rooted at `from` into a contribution to `to`.
 * add_vertex combines all contributions adjacent to `vertex` with the vertex itself.
 *
 * Time: O(N), Memory: O(N).
 * The implementation is iterative and does not use recursive DFS.
 */
#pragma once

#include <cassert>
#include <utility>
#include <vector>

template<class M>
struct Rerooting {
	using DP = typename M::DP;
	using EdgeData = typename M::Edge;

	struct Edge {
		int to;
		EdgeData data;
	};

	int n;
	int edge_count = 0;
	M policy;
	std::vector<std::vector<Edge>> graph;

	explicit Rerooting(int n, M m = M{})
		: n(n), policy(std::move(m)), graph(n) {
		assert(n > 0);
	}

	void add_edge(int u, int v, const EdgeData& x = EdgeData{}) {
		assert(0 <= u && u < n);
		assert(0 <= v && v < n);
		assert(u != v);
		graph[u].push_back({v, x});
		graph[v].push_back({u, x});
		++edge_count;
	}

	std::vector<DP> solve(int r = 0) const {
		assert(0 <= r && r < n);
		assert(edge_count == n - 1);
		const auto& g = graph;
		const auto& m = policy;

		std::vector<int> par(n, -1), ord;
		ord.reserve(n);
		par[r] = r;
		std::vector<int> st = {r};
		while (!st.empty()) {
			int v = st.back();
			st.pop_back();
			ord.push_back(v);
			for (const Edge& e : g[v]) {
				int u = e.to;
				if (u == par[v]) continue;
				assert(par[u] == -1); // graph must be a tree
				par[u] = v;
				st.push_back(u);
			}
		}
		assert((int)ord.size() == n);

		std::vector<DP> dn(n, m.identity());
		for (int i = n - 1; i >= 0; --i) {
			int v = ord[i];
			DP x = m.identity();
			for (const Edge& e : g[v]) {
				int u = e.to;
				if (par[u] != v) continue;
				DP y = m.add_edge(dn[u], e.data, u, v);
				x = m.merge(x, y);
			}
			dn[v] = m.add_vertex(x, v);
		}

		std::vector<DP> up(n, m.identity()), ans(n, m.identity()), pre;
		for (int v : ord) {
			int d = (int)g[v].size();
			auto get = [&](int i) {
				const Edge& e = g[v][i];
				int u = e.to;
				if (u == par[v]) {
					return m.add_edge(up[v], e.data, u, v);
				}
				return m.add_edge(dn[u], e.data, u, v);
			};

			pre.assign(d + 1, m.identity());
			for (int i = 0; i < d; ++i) {
				pre[i + 1] = m.merge(pre[i], get(i));
			}

			ans[v] = m.add_vertex(pre[d], v);
			DP suf = m.identity();
			for (int i = d - 1; i >= 0; --i) {
				int u = g[v][i].to;
				if (par[u] == v) {
					DP x = m.merge(pre[i], suf);
					up[u] = m.add_vertex(x, v);
				}
				suf = m.merge(get(i), suf);
			}
		}

		return ans;
	}
};
