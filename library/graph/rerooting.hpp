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

	explicit Rerooting(int n, M policy = M{})
		: n(n), policy(std::move(policy)), graph(n) {
		assert(n > 0);
	}

	void add_edge(int u, int v, const EdgeData& data = EdgeData{}) {
		assert(0 <= u && u < n);
		assert(0 <= v && v < n);
		assert(u != v);
		graph[u].push_back({v, data});
		graph[v].push_back({u, data});
		++edge_count;
	}

	std::vector<DP> solve(int root = 0) const {
		assert(0 <= root && root < n);
		assert(edge_count == n - 1);

		std::vector<int> parent(n, -1), order;
		order.reserve(n);
		parent[root] = root;
		std::vector<int> stack = {root};
		while (!stack.empty()) {
			int v = stack.back();
			stack.pop_back();
			order.push_back(v);
			for (const Edge& edge : graph[v]) {
				int u = edge.to;
				if (u == parent[v]) continue;
				assert(parent[u] == -1); // graph must be a tree
				parent[u] = v;
				stack.push_back(u);
			}
		}
		assert((int)order.size() == n);

		std::vector<DP> down(n, policy.identity());
		for (int oi = n - 1; oi >= 0; --oi) {
			int v = order[oi];
			DP merged = policy.identity();
			for (const Edge& edge : graph[v]) {
				int u = edge.to;
				if (parent[u] != v) continue;
				DP contribution = policy.add_edge(down[u], edge.data, u, v);
				merged = policy.merge(merged, contribution);
			}
			down[v] = policy.add_vertex(merged, v);
		}

		std::vector<DP> from_parent(n, policy.identity());
		std::vector<DP> answer(n, policy.identity());
		std::vector<DP> prefix;
		for (int v : order) {
			int degree = (int)graph[v].size();
			auto contribution = [&](int i) {
				const Edge& edge = graph[v][i];
				int u = edge.to;
				if (u == parent[v]) {
					return policy.add_edge(from_parent[v], edge.data, u, v);
				}
				return policy.add_edge(down[u], edge.data, u, v);
			};

			prefix.assign(degree + 1, policy.identity());
			for (int i = 0; i < degree; ++i) {
				prefix[i + 1] = policy.merge(prefix[i], contribution(i));
			}

			answer[v] = policy.add_vertex(prefix[degree], v);
			DP suffix = policy.identity();
			for (int i = degree - 1; i >= 0; --i) {
				int u = graph[v][i].to;
				if (parent[u] == v) {
					DP without_u = policy.merge(prefix[i], suffix);
					from_parent[u] = policy.add_vertex(without_u, v);
				}
				suffix = policy.merge(contribution(i), suffix);
			}
		}

		return answer;
	}
};
