// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Binomial coefficients modulo a prime, using factorials and inverse factorials.
 * Requires 0 <= max_n < the prime modulus.
 * Mod can be ModInt<MOD> or KACTL's Mod from ModularArithmetic.h.
 * Only construction from integers, copying, multiplication and division are used.
 * Precomputation: O(max_n + log MOD) time, O(max_n) space. C(n, r): O(1).
 * Valid queries require n <= max_n. Negative n/r or r > n return 0.
 */
#pragma once

#include <cassert>
#include <vector>
#include "modint.hpp"

template<class Mod>
struct Combination {
	std::vector<Mod> fact, ifact;

	explicit Combination(int n) {
		assert(n >= 0);
		// KACTL's Mod has no default constructor.
		fact.assign((std::size_t)n + 1, Mod(0));
		ifact.assign((std::size_t)n + 1, Mod(0));
		fact[0] = 1;
		for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;
		ifact[n] = Mod(1) / fact[n];
		for (int i = n; i > 0; --i) ifact[i - 1] = ifact[i] * i;
	}

	Mod C(int n, int r) const {
		if (n < 0 || r < 0 || r > n) return 0;
		assert(n < (int)fact.size());
		// KACTL's arithmetic operators are not const-qualified.
		return Mod(fact[n]) * ifact[r] * ifact[n - r];
	}
};
