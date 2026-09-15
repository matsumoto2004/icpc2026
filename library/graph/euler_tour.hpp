/**
 * Entry-order Euler tour for a rooted tree.
 * The subtree of v corresponds to [in[v], out[v]) in order.
 * parent[root] == root.
 * Time: O(N), Memory: O(N).
 */
#pragma once

#include <cassert>
#include <utility>
#include <vector>

struct EulerTour {
	int n;
	std::vector<int> in, out, parent, depth, order;

	explicit EulerTour(const std::vector<std::vector<int>>& graph, int root = 0)
		: n((int)graph.size()), in(n, -1), out(n, -1),
		  parent(n, -1), depth(n) {
		assert(0 <= root && root < n);
		order.reserve(n);
		parent[root] = root;
		in[root] = 0;
		order.push_back(root);

		std::vector<std::pair<int, int>> stack = {{root, 0}};
		while (!stack.empty()) {
			int v = stack.back().first;
			int& next = stack.back().second;
			if (next == (int)graph[v].size()) {
				out[v] = (int)order.size();
				stack.pop_back();
				continue;
			}

			int u = graph[v][next++];
			if (u == parent[v]) continue;
			assert(parent[u] == -1); // graph must be a tree
			parent[u] = v;
			depth[u] = depth[v] + 1;
			in[u] = (int)order.size();
			order.push_back(u);
			stack.push_back({u, 0});
		}
		assert((int)order.size() == n);
	}

	std::pair<int, int> subtree_range(int v) const {
		assert(0 <= v && v < n);
		return {in[v], out[v]};
	}

	int subtree_size(int v) const {
		assert(0 <= v && v < n);
		return out[v] - in[v];
	}
};
