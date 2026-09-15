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
	ModInt(long long value) {
		value %= MOD;
		if (value < 0) value += MOD;
		x = (int)value;
	}

	static constexpr int mod() { return MOD; }
	int value() const { return x; }

	ModInt& operator+=(const ModInt& other) {
		x += other.x;
		if (x >= MOD) x -= MOD;
		return *this;
	}

	ModInt& operator-=(const ModInt& other) {
		x -= other.x;
		if (x < 0) x += MOD;
		return *this;
	}

	ModInt& operator*=(const ModInt& other) {
		x = (int)((long long)x * other.x % MOD);
		return *this;
	}

	ModInt& operator/=(const ModInt& other) {
		return *this *= other.inv();
	}

	ModInt operator+() const { return *this; }
	ModInt operator-() const { return ModInt(-x); }

	friend ModInt operator+(ModInt left, const ModInt& right) {
		return left += right;
	}
	friend ModInt operator-(ModInt left, const ModInt& right) {
		return left -= right;
	}
	friend ModInt operator*(ModInt left, const ModInt& right) {
		return left *= right;
	}
	friend ModInt operator/(ModInt left, const ModInt& right) {
		return left /= right;
	}
	friend bool operator==(const ModInt& left, const ModInt& right) {
		return left.x == right.x;
	}
	friend bool operator!=(const ModInt& left, const ModInt& right) {
		return !(left == right);
	}

	ModInt pow(long long exponent) const {
		assert(exponent >= 0);
		ModInt base = *this, result = 1;
		while (exponent) {
			if (exponent & 1) result *= base;
			base *= base;
			exponent >>= 1;
		}
		return result;
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

	ModInt operator^(long long exponent) const { return pow(exponent); }
	// Same calling convention as KACTL's Mod::invert(a).
	ModInt invert(ModInt a) const { return a.inv(); }

	friend std::ostream& operator<<(std::ostream& out, const ModInt& value) {
		return out << value.x;
	}
	friend std::istream& operator>>(std::istream& in, ModInt& value) {
		long long x;
		in >> x;
		value = ModInt(x);
		return in;
	}
};
