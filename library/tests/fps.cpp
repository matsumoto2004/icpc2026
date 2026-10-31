#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "../math/modint.hpp"
#include "../math/fps.hpp"
using V = fps;
void equal(const V& a, const V& b) {
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i) assert(a[i].x == b[i].x);
}
V naive(const V& a, const V& b, int n) {
    V c(n, Mod(0));
    for (int i = 0; i < (int)a.size() && i < n; ++i)
        for (int j = 0; j < (int)b.size() && i + j < n; ++j)
            c[i+j] = c[i+j] + Mod(a[i]) * b[j];
    return c;
}
int main() {
    static_assert(is_base_of_v<vector<Mod>, fps>);
    V zero(4); equal(zero, V{0,0,0,0});
    zero.resize(6); equal(zero, V{0,0,0,0,0,0});
    vector<Mod> raw{1,2}; V converted = raw;
    converted *= V{1,1}; equal(converted, V{1,3,2});
    const V fixed{1,2}; equal(fixed.prefix(3), V{1,2,0});
    mt19937 rng(123456);
    for (int n : {1, 2, 3, 7, 32, 33, 64, 65, 127, 256}) {
        V a(n, Mod(0)), b(n + 3, Mod(0));
        for (auto& x : a) x = rng() % mod;
        for (auto& x : b) x = rng() % mod;
        equal(a*b, naive(a,b,(int)(a.size()+b.size()-1)));
        V inverse(n, Mod(0)); inverse[0] = Mod(1) / a[0];
        for (int i=1;i<n;++i) {
            Mod sum(0);
            for(int j=1;j<=i;++j) sum = sum + a[j] * inverse[i-j];
            inverse[i] = (Mod(0)-sum) / a[0];
        }
        equal(a.inv(n), inverse);
        equal(b.div(a,n),naive(b,inverse,n));
        equal(a.integral().derivative(),a);
        a[0]=0;
        V e(n, Mod(0)); e[0]=1;
        for(int i=1;i<n;++i) {
            Mod sum(0);
            for(int j=1;j<=i;++j) sum=sum+Mod(j)*a[j]*e[i-j];
            e[i]=sum/Mod(i);
        }
        equal(a.exp(n),e);
        equal(e.log(n),a);
        for (int leading : {0, 1, 3}) {
            V f=a;
            f[0]=2;
            for(int i=0;i<min(n,leading);++i) f[i]=0;
            V p(n,Mod(0));p[0]=1;
            for(int k=0;k<=6;++k) {
                equal(f.pow(k,n),p);
                p=naive(p,f,n);
            }
        }
    }
    equal(V{} * V{1},V{});
    equal(V{}.inv(0),V{});
    equal(V{}.exp(3),V{1,0,0});
    equal(V{}.pow(0,3),V{1,0,0});
    equal(V{}.pow(4,3),V{0,0,0});
    equal(V{0,1}.pow(LLONG_MAX,3),V{0,0,0});
    equal(V{1}.pow(LLONG_MAX,3),V{1,0,0});
    equal(V{1}.pow(1,0),V{});
    cout << "fps checks passed\n";
}
