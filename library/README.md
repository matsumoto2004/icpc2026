# C++ Library

チームで共有する競技プログラミング用C++ライブラリです。KACTLは参照用として保持し、理解・検証した再利用コードはこのディレクトリへ置きます。

コンパイル時に `-I` でリポジトリルートをinclude pathへ追加すると、解答から次の形式で参照できます。リポジトリルートで実行する場合は `-I.` を指定してください。

```cpp
#include "library/graph/euler_tour.hpp"
```

## Data Structure

### 座標圧縮

[coordinate_compression.hpp](data_structure/coordinate_compression.hpp)

複数の入力をまとめて登録でき、圧縮後の添字から元の値へ戻せます。
KACTLの `Point<T>` も圧縮でき、`all(x)` マクロが定義された後でもincludeできます。

```cpp
Compress<long long> comp;
comp.add(a);
comp.add(b);
comp.build();

int i = comp.rank(x);
long long x2 = comp[i];
```

### 汎用Lazy Segment Tree

[generic_lazy_segment_tree.hpp](data_structure/generic_lazy_segment_tree.hpp)

操作を表す型 `M` を問題ごとに定義します。すべての区間は `[l, r)` です。

```cpp
struct M {
    struct S { long long sum, size; };
    using F = long long;

    static S op(S a, S b) {
        return {a.sum + b.sum, a.size + b.size};
    }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) {
        return {x.sum + f * x.size, x.size};
    }
    static F composition(F f, F g) {
        return f + g;
    }
    static F id() { return 0; }
};

vector<M::S> initial(n, {0, 1});
LazySegmentTree<M> tree(initial);
tree.apply(l, r, x);
long long sum = tree.query(l, r).sum;
```

`composition(f, g)`は、`g`を適用した後に`f`を適用する操作を返します。

### 転倒数

[inversion_count.hpp](data_structure/inversion_count.hpp)

重複を転倒として数えず、`i < j`かつ`a[i] > a[j]`である組を数えます。

```cpp
long long answer = inversion_count(a);
```

## Graph

### 全方位木DP

[rerooting.hpp](graph/rerooting.hpp)

結合則を満たす部分木DPを、すべての頂点を根とした場合について `O(N)` で計算します。再帰は使用しません。

問題ごとに `DP`, `Edge` と4つの操作を定義します。次は各頂点から全頂点までの距離の総和を求める例です。

```cpp
struct M {
    struct DP { long long distance_sum, size; };
    using Edge = int;

    DP identity() const { return {0, 0}; }
    DP merge(DP a, DP b) const {
        return {a.distance_sum + b.distance_sum, a.size + b.size};
    }
    DP add_edge(DP x, const Edge& length, int from, int to) const {
        (void)from;
        (void)to;
        return {x.distance_sum + x.size * length, x.size};
    }
    DP add_vertex(DP x, int vertex) const {
        (void)vertex;
        return {x.distance_sum, x.size + 1};
    }
};

Rerooting<M> tree(n);
tree.add_edge(u, v, 1);
vector<M::DP> answer = tree.solve();
cout << answer[root].distance_sum << '\n';
```

`add_edge(value, edge, from, to)` は、`from` を根とする側のDPを辺越しに `to` へ渡す変換です。

### 木のEuler Tour

[euler_tour.hpp](graph/euler_tour.hpp)

根付き木を行きがけ順に平坦化します。頂点`v`の部分木は `[in[v], out[v])` です。

```cpp
EulerTour tour(graph, 0);
auto [l, r] = tour.subtree_range(v);
int vertex = tour.order[i];
```

これは各辺を1回ずつ通るKACTLの`EulerWalk.h`とは別のものです。

## Math

### ModInt

[modint.hpp](math/modint.hpp)

法をコンパイル時に指定します。除算は除数と法が互いに素な場合に使用できます。

KACTL式の `a ^ exponent` と `a.invert(b)` にも対応します。KACTLの `Matrix` の要素型としても使用できます。

```cpp
using mint = ModInt<998244353>;

mint a = 10;
mint b = a * a + 3;
mint c = b / 2;
cout << c << '\n';
```

### 二項係数（組合せ）

[combination.hpp](math/combination.hpp)

階乗・逆階乗を前計算し、`nCr` を指定した法で求めます。`modint.hpp` も自動でincludeされます。

```cpp
#include "library/math/combination.hpp"

using mint = ModInt<998244353>;
Combination<mint> comb(1000000);  // n = 0 から 1000000 まで前計算

cout << comb.C(5, 2) << '\n';    // 10
cout << comb.C(0, 0) << '\n';    // 1
cout << comb.C(5, 6) << '\n';    // 0
```

法は素数を使用し、前計算の上限 `N` は `0 <= N < MOD` にしてください。各クエリの `n` は前計算した範囲内である必要があります。`n < 0`、`r < 0`、`r > n` の場合は `0` を返します。

KACTLの `ModularArithmetic.h` の `Mod` もそのまま使用できます。KACTL用の `ll` などを定義したテンプレートで、次のようにincludeしてください。

```cpp
#include "kactl/content/number-theory/ModularArithmetic.h"
#include "library/math/combination.hpp"

// KACTLの現在の法は17。問題に合わせて取り込む際に設定します。
const Combination<Mod> comb(16);
cout << comb.C(5, 2).x << '\n';  // 10
```

`Combination` はデフォルトコンストラクタや `.inv()` を要求しません。整数からの構築、コピー、乗算、除算に対応した型を渡せます。実行中に法を変更する実装では、法を変更するたびに前計算を作り直してください。

前計算は `O(N + log MOD)` 時間・`O(N)` メモリ、`C(n, r)` は `O(1)` です。`fact[i]` は `i!`、`ifact[i]` はその逆元を保持します。提出時にコピペする場合は、`ModInt` と `Combination` の両方をコピーしてください。

## Misc

### デバッグ出力

[debug.hpp](misc/debug.hpp)

```cpp
#include "library/misc/debug.hpp"

debug(values);
```

`-DLOCAL`付きのときだけ標準エラーへ出力します。通常ビルドでは何も行いません。スカラー、KACTLの`Mod`、`pair`、`vector`（入れ子を含む）の出力に対応しています。ルートの`debug.hpp`も同じ実装を参照します。

### 再現可能な乱数

[random.hpp](misc/random.hpp)

```cpp
Random random(seed);
int x = random.range(0, 10);       // [0, 10)
int y = random.inclusive(0, 10);   // [0, 10]
bool b = random.boolean();
random.shuffle(a.begin(), a.end());
```

同じseedからは同じ乱数列が得られるため、ランダムチェックの入力生成に使用できます。

### 2進文字列

[to_binary_string.hpp](misc/to_binary_string.hpp)

```cpp
to_binary_string(10);     // "1010"
to_binary_string(10, 8);  // "00001010"
```

入力は非負整数です。第2引数は先頭を`0`で埋めた最小幅です。

## 追加方針

互換性テストはKACTLのsubmoduleを取得したうえで、リポジトリルートから実行します。

```sh
git submodule update --init --recursive
mkdir -p .build
g++ -std=c++20 -O2 -Wall -Wextra -DLOCAL -I. library/tests/kactl_compatibility.cpp -o .build/kactl_compatibility
./.build/kactl_compatibility
```

- 必要になったものだけ追加する
- KACTLなどの実装を理解してから自分用に書く
- ヘッダ単体でincludeできるようにする
- 区間の端点、計算量、重複の扱いなどを先頭コメントへ書く
- 愚直解やランダムテストで確認してから使用する
