// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Reproducible random-number helper for generators and stress tests.
 * range(l, r) returns a value in [l, r), and inclusive(l, r) in [l, r].
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <random>

struct Random {
	std::mt19937_64 engine;

	explicit Random(std::uint64_t s) : engine(s) {}

	std::uint64_t next_u64() { return engine(); }

	int range(int r) {
		assert(0 < r);
		return range(0, r);
	}

	int range(int l, int r) {
		assert(l < r);
		return std::uniform_int_distribution<int>(l, r - 1)(engine);
	}

	long long range(long long r) {
		assert(0 < r);
		return range(0LL, r);
	}

	long long range(long long l, long long r) {
		assert(l < r);
		return std::uniform_int_distribution<long long>(l, r - 1)(engine);
	}

	int inclusive(int l, int r) {
		assert(l <= r);
		return std::uniform_int_distribution<int>(l, r)(engine);
	}

	long long inclusive(long long l, long long r) {
		assert(l <= r);
		return std::uniform_int_distribution<long long>(l, r)(engine);
	}

	bool boolean() {
		return std::uniform_int_distribution<int>(0, 1)(engine);
	}

	double real(double l = 0.0, double r = 1.0) {
		assert(l < r);
		return std::uniform_real_distribution<double>(l, r)(engine);
	}

	template<class It>
	void shuffle(It l, It r) {
		std::shuffle(l, r, engine);
	}
};
