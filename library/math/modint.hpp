// Source: https://github.com/matsumoto2004/icpc2026
// Adapted from KACTL: ModularArithmetic.h (Lukas Polacek, Joshua Andersson; CC0)
// and euclid.h (Author: Unknown).
// https://github.com/kth-competitive-programming/kactl/blob/fb67e471a878eb06ad9ed036c38c76b6c913847c/content/number-theory/ModularArithmetic.h
// https://github.com/kth-competitive-programming/kactl/blob/fb67e471a878eb06ad9ed036c38c76b6c913847c/content/number-theory/euclid.h
ll euclid(ll a, ll b, ll &x, ll &y) {
	if (!b) return x = 1, y = 0, a;
	ll d = euclid(b, a % b, y, x);
	return y -= a/b * x, d;
}

const ll mod = 998244353; // change to something else
struct Mod {
	ll x;
	Mod(ll y) : Mod(y%mod+mod,0){}
	Mod(ll y,int) : x(y<mod?y:y-mod){}
	Mod operator+(Mod b) { return {x + b.x,0}; }
	Mod operator-(Mod b) { return {x - b.x + mod,0}; }
	Mod operator*(Mod b) { return {x * b.x % mod,0}; }
	Mod operator/(Mod b) { return *this * invert(b); }
	Mod invert(Mod a) {
		ll x, y, g = euclid(a.x, mod, x, y);
		assert(g == 1); return x;
	}
	Mod operator^(ll e) {
		if (!e) return 1;
		Mod r = *this ^ (e / 2); r = r * r;
		return e&1 ? *this * r : r;
	}
};
