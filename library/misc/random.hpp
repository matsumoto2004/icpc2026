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

	explicit Random(std::uint64_t seed) : engine(seed) {}

	std::uint64_t next_u64() { return engine(); }

	int range(int upper) {
		assert(0 < upper);
		return range(0, upper);
	}

	int range(int lower, int upper) {
		assert(lower < upper);
		return std::uniform_int_distribution<int>(lower, upper - 1)(engine);
	}

	long long range(long long upper) {
		assert(0 < upper);
		return range(0LL, upper);
	}

	long long range(long long lower, long long upper) {
		assert(lower < upper);
		return std::uniform_int_distribution<long long>(lower, upper - 1)(engine);
	}

	int inclusive(int lower, int upper) {
		assert(lower <= upper);
		return std::uniform_int_distribution<int>(lower, upper)(engine);
	}

	long long inclusive(long long lower, long long upper) {
		assert(lower <= upper);
		return std::uniform_int_distribution<long long>(lower, upper)(engine);
	}

	bool boolean() {
		return std::uniform_int_distribution<int>(0, 1)(engine);
	}

	double real(double lower = 0.0, double upper = 1.0) {
		assert(lower < upper);
		return std::uniform_real_distribution<double>(lower, upper)(engine);
	}

	template<class RandomAccessIterator>
	void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
		std::shuffle(first, last, engine);
	}
};
