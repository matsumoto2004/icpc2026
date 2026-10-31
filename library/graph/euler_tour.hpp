// Source: https://github.com/matsumoto2004/icpc2026
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

	explicit EulerTour(const std::vector<std::vector<int>>& g, int r = 0)
		: n((int)g.size()), in(n, -1), out(n, -1),
		  parent(n, -1), depth(n) {
		assert(0 <= r && r < n);
		order.reserve(n);
		parent[r] = r;
		in[r] = 0;
		order.push_back(r);

		std::vector<std::pair<int, int>> st = {{r, 0}};
		while (!st.empty()) {
			int v = st.back().first;
			int& i = st.back().second;
			if (i == (int)g[v].size()) {
				out[v] = (int)order.size();
				st.pop_back();
				continue;
			}

			int u = g[v][i++];
			if (u == parent[v]) continue;
			assert(parent[u] == -1); // graph must be a tree
			parent[u] = v;
			depth[u] = depth[v] + 1;
			in[u] = (int)order.size();
			order.push_back(u);
			st.push_back({u, 0});
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
