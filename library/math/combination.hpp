/**
 * Binomial coefficients modulo a prime, using factorials and inverse factorials.
 * Requires 0 <= max_n < the prime modulus.
 * Mint can be ModInt<MOD> or KACTL's Mod from ModularArithmetic.h.
 * Only construction from integers, copying, multiplication and division are used.
 * Precomputation: O(max_n + log MOD) time, O(max_n) space. C(n, r): O(1).
 * Valid queries require n <= max_n. Negative n/r or r > n return 0.
 */
#pragma once

#include <cassert>
#include <vector>
#include "modint.hpp"


template<class Mint>
struct Combination {
	std::vector<Mint> fact, ifact;

	explicit Combination(int max_n) {
		assert(max_n >= 0);
		// KACTL's Mod has no default constructor.
		fact.assign((std::size_t)max_n + 1, Mint(0));
		ifact.assign((std::size_t)max_n + 1, Mint(0));
		fact[0] = 1;
		for (int i = 1; i <= max_n; ++i) fact[i] = fact[i - 1] * i;
		ifact[max_n] = Mint(1) / fact[max_n];
		for (int i = max_n; i > 0; --i) ifact[i - 1] = ifact[i] * i;
	}

	Mint C(int n, int r) const {
		if (n < 0 || r < 0 || r > n) return 0;
		assert(n < (int)fact.size());
		// KACTL's arithmetic operators are not const-qualified.
		return Mint(fact[n]) * ifact[r] * ifact[n - r];
	}
};
