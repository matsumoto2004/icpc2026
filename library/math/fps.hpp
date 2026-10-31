// Source: https://github.com/matsumoto2004/icpc2026
/**
 * Formal power series modulo 998244353, fps inherits vector<Mod> (low degree first).
 * Define Mod before use; this header deliberately does not include modint.hpp.
 * Compatible with KACTL's Mod (no default constructor / const operators).
 * Mod must use modulus 998244353 and expose its normalized value as .x.
 * prefix/inv/div/log/exp/pow return exactly n coefficients, modulo x^n.
 * inv: f[0] != 0; log: f[0] == 1; exp: f[0] == 0 (empty allowed).
 * pow supports nonnegative long long exponents, including 0^0 = 1.
 * Multiplication and Newton operations: O(n log n); derivative/integral: O(n).
 * NTT length must be <= 2^23; intermediate products must also fit this limit.
 * Integral requires f.size() < 998244353. Preconditions checked by assert.
 */
#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>
#include <utility>

struct fps : std::vector<Mod> {
private:
    static Mod pw(Mod a, long long k) {
        Mod r(1);
        for (; k; k >>= 1, a = a * a)
            if (k & 1) r = r * a;
        return r;
    }

    static void ntt(std::vector<Mod>& a, bool inv) {
        int n = (int)a.size();
        assert(n > 0 && (n & (n - 1)) == 0 && n <= (1 << 23));
        for (int i = 1, j = 0; i < n; ++i) {
            int b = n >> 1;
            for (; j & b; b >>= 1) j ^= b;
            j ^= b;
            if (i < j) std::swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            Mod rt = pw(Mod(3), 998244352 / len);
            if (inv) rt = pw(rt, 998244351);
            for (int i = 0; i < n; i += len) {
                Mod w(1);
                for (int j = 0; j < len / 2; ++j) {
                    Mod u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w = w * rt;
                }
            }
        }
        if (inv) {
            Mod z = pw(Mod(n), 998244351);
            for (auto& x : a) x = x * z;
        }
    }
public:
    using Base = std::vector<Mod>;
    using Base::vector;
    using Base::operator=;
    fps() = default;
    explicit fps(std::size_t n) : Base(n, Mod(0)) {}
    fps(const Base& a) : Base(a) {}
    fps(Base&& a) : Base(std::move(a)) {}
    // Mod has no default constructor; growing an fps always zero-fills.
    void resize(std::size_t n, Mod x = Mod(0)) { Base::resize(n, x); }

    fps prefix(int n) const {
        const auto& f = *this;
        assert(n >= 0);
        fps r(f.begin(), f.begin() + std::min(f.size(), (std::size_t)n));
        r.resize(n, Mod(0));
        return r;
    }

    fps operator*(const fps& b) const {
        const auto& a = *this;
        if (a.empty() || b.empty()) return {};
        std::size_t m = a.size() + b.size() - 1;
        assert(m <= (1 << 23));
        if (std::min(a.size(), b.size()) <= 32) {
            fps c(m, Mod(0));
            for (std::size_t i = 0; i < a.size(); ++i)
                for (std::size_t j = 0; j < b.size(); ++j)
                    c[i + j] = c[i + j] + Mod(a[i]) * b[j];
            return c;
        }
        int n = 1;
        while ((std::size_t)n < m) n <<= 1;
        auto x = a.prefix(n), y = b.prefix(n);
        ntt(x, false);
        ntt(y, false);
        for (int i = 0; i < n; ++i) x[i] = x[i] * y[i];
        ntt(x, true);
        x.resize(m, Mod(0));
        return x;
    }

    fps derivative() const {
        const auto& f = *this;
        if (f.empty()) return {};
        fps r(f.size() - 1, Mod(0));
        for (std::size_t i = 1; i < f.size(); ++i) r[i - 1] = Mod(f[i]) * Mod(i);
        return r;
    }

    fps integral() const {
        const auto& f = *this;
        assert(f.size() < 998244353);
        fps r(f.size() + 1, Mod(0)), iv(f.size() + 1, Mod(0));
        if (!f.empty()) iv[1] = 1;
        for (std::size_t i = 2; i <= f.size(); ++i)
            iv[i] = Mod(0) - Mod(998244353 / i) * iv[998244353 % i];
        for (std::size_t i = 0; i < f.size(); ++i) r[i + 1] = Mod(f[i]) * iv[i + 1];
        return r;
    }

    fps inv(int n) const {
        const auto& f = *this;
        assert(n >= 0);
        if (!n) return {};
        assert(!f.empty() && f[0].x != 0);
        fps g{Mod(1) / f[0]};
        while ((int)g.size() < n) {
            int m = (int)std::min<long long>(2LL * (long long)g.size(), n);
            auto t = (f.prefix(m) * g).prefix(m);
            for (auto& x : t) x = Mod(0) - x;
            t[0] = t[0] + Mod(2);
            g = (g * t).prefix(m);
        }
        return g;
    }

    // fps division (not polynomial quotient/remainder).
    fps div(const fps& g, int n) const {
        return (prefix(n) * g.inv(n)).prefix(n);
    }

    fps log(int n) const {
        const auto& f = *this;
        assert(n >= 0);
        if (!n) return {};
        assert(!f.empty() && f[0].x == 1);
        auto a = f.prefix(n);
        return (a.derivative() * a.inv(n)).prefix(n - 1).integral();
    }

    fps exp(int n) const {
        const auto& f = *this;
        assert(n >= 0);
        if (!n) return {};
        assert(f.empty() || f[0].x == 0);
        fps g{Mod(1)};
        while ((int)g.size() < n) {
            int m = (int)std::min<long long>(2LL * (long long)g.size(), n);
            auto t = f.prefix(m), lg = g.log(m);
            for (int i = 0; i < m; ++i) t[i] = t[i] - lg[i];
            t[0] = t[0] + Mod(1);
            g = (g * t).prefix(m);
        }
        return g;
    }

    fps pow(long long k, int n) const {
        const auto& f = *this;
        assert(k >= 0 && n >= 0);
        fps r(n, Mod(0));
        if (!n) return r;
        if (!k) { r[0] = 1; return r; }
        int l = 0;
        while (l < n && (std::size_t)l < f.size() && f[l].x == 0) ++l;
        if (l == n || (std::size_t)l == f.size()) return r;
        if (l && k > (n - 1) / l) return r;
        int s = (int)(l * k), m = n - s;
        Mod c = f[l], ic = Mod(1) / c;
        fps a(m, Mod(0));
        for (int i = 0; i < m && (std::size_t)(l + i) < f.size(); ++i)
            a[i] = Mod(f[l + i]) * ic;
        auto t = a.log(m);
        for (auto& x : t) x = x * Mod(k);
        t = t.exp(m);
        Mod z = pw(c, k);
        for (int i = 0; i < m; ++i) r[s + i] = t[i] * z;
        return r;
    }
    fps& operator*=(const fps& b) {
        return *this = *this * b;
    }
};
