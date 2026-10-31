// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Compile-time modular integer.
 * Division is valid when the divisor and MOD are coprime.
 */
#pragma once

#include <cassert>
#include <iostream>
#include <utility>

template<int MOD>
struct ModInt {
	static_assert(1 <= MOD);

	int x = 0;

	ModInt() = default;
	ModInt(long long v) {
		v %= MOD;
		if (v < 0) v += MOD;
		x = (int)v;
	}

	static constexpr int mod() { return MOD; }
	int value() const { return x; }

	ModInt& operator+=(const ModInt& b) {
		x += b.x;
		if (x >= MOD) x -= MOD;
		return *this;
	}

	ModInt& operator-=(const ModInt& b) {
		x -= b.x;
		if (x < 0) x += MOD;
		return *this;
	}

	ModInt& operator*=(const ModInt& b) {
		x = (int)((long long)x * b.x % MOD);
		return *this;
	}

	ModInt& operator/=(const ModInt& b) {
		return *this *= b.inv();
	}

	ModInt operator+() const { return *this; }
	ModInt operator-() const { return ModInt(-x); }

	friend ModInt operator+(ModInt a, const ModInt& b) { return a += b; }
	friend ModInt operator-(ModInt a, const ModInt& b) { return a -= b; }
	friend ModInt operator*(ModInt a, const ModInt& b) { return a *= b; }
	friend ModInt operator/(ModInt a, const ModInt& b) { return a /= b; }
	friend bool operator==(const ModInt& a, const ModInt& b) { return a.x == b.x; }
	friend bool operator!=(const ModInt& a, const ModInt& b) { return !(a == b); }

	ModInt pow(long long k) const {
		assert(k >= 0);
		ModInt a = *this, r = 1;
		while (k) {
			if (k & 1) r *= a;
			a *= a;
			k >>= 1;
		}
		return r;
	}

	ModInt inv() const {
		long long a = x, b = MOD, u = 1, v = 0;
		while (b) {
			long long q = a / b;
			a -= q * b;
			std::swap(a, b);
			u -= q * v;
			std::swap(u, v);
		}
		assert(a == 1);
		return ModInt(u);
	}

	ModInt operator^(long long k) const { return pow(k); }
	// Same calling convention as KACTL's Mod::invert(a).
	ModInt invert(ModInt a) const { return a.inv(); }

	friend std::ostream& operator<<(std::ostream& os, const ModInt& v) {
		return os << v.x;
	}
	friend std::istream& operator>>(std::istream& is, ModInt& v) {
		long long x;
		is >> x;
		v = ModInt(x);
		return is;
	}
};
