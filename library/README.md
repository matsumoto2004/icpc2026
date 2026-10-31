# C++ Library

必要な実装をコピペして、各自のコードに合わせて使ってください。使用条件や計算量は各ファイルのコメントを参照してください。
提出時は `// Source: https://github.com/matsumoto2004/icpc2026` を残してください。事前公開・公開範囲の運用は [ルートREADME](../README.md#atcoderでの使用と出典) を参照してください。

`kactl/` の有効なコードは原本として保持します。用例や補助関数を追記する場合はコメントアウトした状態にします。KACTLにない機能や、演算・型を差し替えられる汎用版を `library/` に追加します。
ICPCで書き写しやすいよう、内部変数には `a`・`res`・`par`・`ord`・`pre` などの短い名前を使います。既存の関数名・公開メンバー名・動作・計算量は保ち、単純な関数は短く記述します。

## Data Structure

- [座標圧縮](data_structure/coordinate_compression.hpp)（[書き方・用例](data_structure/coordinate_compression.md)）
- [汎用Lazy Segment Tree](data_structure/generic_lazy_segment_tree.hpp)
- [転倒数](data_structure/inversion_count.hpp)

### 汎用Lazy Segment Tree

`LazySegmentTree<Policy>` は、値の型・集約・遅延操作をPolicyで指定します。
区間はすべて半開区間 `[l, r)` です。実際に使うPolicyは解答側で定義してください。

- `LazySegmentTree<Policy> seg(values)`：配列から構築
- `LazySegmentTree<Policy> seg(n)`：`[0, n)` を `Policy::e()` で初期化
- `LazySegmentTree<Policy> seg(lo, hi)`：`[lo, hi)` を `Policy::e()` で初期化。操作にもこの座標を使う
- `seg.set(p, value)` / `seg.get(p)`：1点の代入・取得
- `seg.query(l, r)` / `seg.all_query()`：区間・全体の集約
- `seg.max_right(l, g)`：`g(seg.query(l, r))` がtrueになる最大のrをO(log N)で探す
- `seg.apply(l, r, operation)` / `seg.apply(p, operation)`：区間・1点への遅延操作
- `seg.add(l, r, operation)`：`apply(l, r, operation)` の別名。KACTLのHLDとの接続用

`composition(f, g)` は「gを適用してからfを適用する」合成です。
`max_right` は `g(Policy::e()) == true` が必要です。rを増やしたとき判定がtrueからfalseに単調に変わる条件を指定してください。全区間でtrueなら右端を返します。座標を指定して構築した場合、返り値もその座標です。
構築O(N)、各操作O(log N)、`all_query()` はO(1)、メモリO(N)です。

例えば区間加算・区間総和は、次のPolicyで使えます（`ll` は既存テンプレートの型定義）。

```cpp
struct SumAdd {
    struct S { ll sum; int len; };
    using F = ll;
    static S op(S a, S b) { return {a.sum + b.sum, a.len + b.len}; }
    static S e() { return {0, 0}; }
    static S mapping(F f, S x) { return {x.sum + f * x.len, x.len}; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

// main内。各要素のlenは1で初期化する。
vector<SumAdd::S> a(n, {0, 1});
LazySegmentTree<SumAdd> seg(a);
```

空き位置を1、使用済みを0として区間和を持てば、`max_right` で左からk番目の空き（1始まり）を探せます。
`[0, r)` の空き数がk未満である最大のrが、その空き位置の添字になります。

```cpp
LazySegmentTree<SumAdd> seg(vector<SumAdd::S>(n, {1, 1}));
// 1 <= k <= seg.all_query().sum
int p = seg.max_right(0, [&](SumAdd::S x) { return x.sum < k; });
seg.set(p, {0, 1});
```

この探索では各要素の空き数が非負であることが必要です。

### KACTLの通常Segment Treeで区間境界を探す

[KACTLのSegmentTree.h](https://github.com/kth-competitive-programming/kactl/blob/fb67e471a878eb06ad9ed036c38c76b6c913847c/content/data-structures/SegmentTree.h) のTree内に、次の汎用探索を追加して使えます。KACTL原本の著者はLucian Bicsi、ライセンスはCC0です。

- `max_right(l, g)`：`g(query(l, r))` がtrueになる最大のr。全区間でtrueならn
- `min_left(r, g)`：`g(query(l, r))` がtrueになる最小のl。全区間でtrueなら0

必要な関数を解答へコピーします。`g(unit) == true` が必要で、区間を広げるにつれて判定がtrueからfalseへ変わる条件を使います。
どちらも `query` を判定に使う通常の二分探索なのでO(log² N)です。任意のnで使え、2のべき乗への丸めは不要です。

```cpp
// Source: https://github.com/matsumoto2004/icpc2026
// KACTLのTree内に追加する。
template<class G>
int max_right(int l, G g) {
    assert(0 <= l && l <= n && g(unit));
    int a = l, b = n;
    while (a < b) {
        int m = a + (b - a + 1) / 2;
        if (g(query(l, m))) a = m;
        else b = m - 1;
    }
    return a;
}

template<class G>
int min_left(int r, G g) {
    assert(0 <= r && r <= n && g(unit));
    int a = 0, b = r;
    while (a < b) {
        int m = a + (b - a) / 2;
        if (g(query(m, r))) b = m;
        else a = m + 1;
    }
    return a;
}
```

```cpp
// 元の最大値用Tree：l以降で初めて値がx以上になる添字。なければn。
int p = seg.max_right(l, [&](int v) { return v < x; });
```

区間和用に `unit = 0`、`f(a, b) = a + b` とした場合、非負の空き数を用いて次のようにk番目（1始まり）の空きを探せます。

```cpp
int p = seg.max_right(0, [&](int v) { return v < k; });
seg.update(p, 0); // k <= 空き総数の場合。見つからなければpはn。
```

ICPCでもO(log² N)の方法を使えます。探索Q回なら全体はO(Q log² N)なので、入力上限・制限時間・1回のqueryのコストを見て選びます。
木を直接降りるO(log N)版が必要な場合は、汎用Lazy Segment Treeの `max_right`、または以下の区間和専用の `kth` を使えます。

区間和専用の `kth(k)` は次のとおりです。解答へコピーするときは `unit = 0`、`f(a, b) = a + b` に変更し、`kth` をTree内に追加します。
この `kth` は根から左右へ降りるため、Treeの容量を実際の要素数以上の2のべき乗にします。通常の `query` 自体には、この容量の制約はありません。

```cpp
// Source: https://github.com/matsumoto2004/icpc2026
// KACTLのTree内。unitとfを次の2つに置き換え、kthを追加する。
static constexpr T unit = 0;
T f(T a, T b) { return a + b; }

int kth(T k) {
    assert(1 <= k && k <= s[1]);
    int p = 1;
    while (p < n) {
        if (k <= s[p * 2]) p *= 2;
        else k -= s[p * 2], p = p * 2 + 1;
    }
    return p - n;
}

// main内。余った葉はunit=0のままにする。
int m = 1;
while (m < N) m *= 2;
Tree seg(m);
for (int i = 0; i < N; ++i) seg.update(i, 1);
int p = seg.kth(k); // kは1始まり
seg.update(p, 0);
```

空き位置を1、使用済みを0として初期化し、左からk番目（1始まり）を `seg.kth(k)` で探します。使った位置は `seg.update(p, 0)` で消します。
遅延更新を持たないので `push` は不要です。探索はO(log N)です。

### KACTLのHLDとの組み合わせ

KACTL本体はそのまま保持し、解答へコピーしたHLDの `Node` としてこの汎用版を使えます。
KACTLの `LazySegmentTree.h` を含める代わりに、次のヘッダ・Policy・型エイリアスをHLDの定義より前に置きます。
既存テンプレートの型定義・マクロの後に配置してください。

```cpp
#include "library/data_structure/generic_lazy_segment_tree.hpp"

struct MinAdd {
    using S = int;
    using F = int;
    static S op(S a, S b) { return min(a, b); }
    static S e() { return numeric_limits<S>::max() / 4; }
    static S mapping(F f, S x) { return x == e() ? x : x + f; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};

using Node = LazySegmentTree<MinAdd>;

// この後にKACTLの template<bool VALS_EDGES> struct HLD { ... }; をコピーする。
// KACTL側の LazySegmentTree.h は含めない。
```

`Node(0, N)` と `tree->add(l, r, value)` が使えるため、コピーしたHLD本体はそのままで接続できます。
初期値はHLDの並び順に並べた配列から構築するとO(N)です。次はmain内の使用例です。

```cpp
// graphは構築済みの無向木の隣接リスト。根は0。
HLD<false> hld(graph);
vector<int> a(graph.size());
for (int v = 0; v < (int)graph.size(); ++v)
    a[hld.pos[v]] = hld.siz[v]; // 例：部分木ごとの余裕
*hld.tree = Node(a);

// 根〜vの余裕の最小値
int mn = MinAdd::e();
hld.process(0, v, [&](int l, int r) {
    mn = min(mn, hld.tree->query(l, r));
});

// 根〜vの余裕を1減らす。HLD内のaddが汎用版のapplyに対応する。
if (mn > 0) hld.modifyPath(0, v, -1);

// 部分木全体の最小値
int mn_sub = hld.querySubtree(v);
```

`querySubtree(v)` はvを根とする部分木の集約、`modifySubtree(v, x)` はその部分木への更新です。
MinAddではそれぞれ「部分木内の最小値」と「部分木全体への加算」になります。
KACTLのHLDには前者だけ定義されています。後者も使う場合は、解答へコピーしたHLDに次のように書けます。

```cpp
int querySubtree(int v) {
    return tree->query(pos[v] + VALS_EDGES, pos[v] + siz[v]);
}
void modifySubtree(int v, int x) {
    tree->add(pos[v] + VALS_EDGES, pos[v] + siz[v], x);
}
```

HLDのDFS順では、部分木が `[pos[v], pos[v] + siz[v])` の連続区間になります。
`VALS_EDGES` はHLDのboolテンプレート引数で、`false`（0）は頂点に、`true`（1）は辺に値を持つ指定です。J問題では `HLD<false>` を使います。
辺の値は子側の `pos` に置きます。`HLD<false>` はv自身を含み、`HLD<true>` はvと親を結ぶ辺を除外して、部分木内の辺を対象にします。
どちらの操作もO(log N)です。根からvまでのパスを更新する場合は `modifyPath(0, v, x)` を使います。

この接続例はKACTLのHLDに合わせて値型をintにしています。汎用セグ木単体ではlong longや独自structも使用できますが、HLDに接続する場合はHLD内の固定された型も合わせる必要があります。
KACTLの `queryPath` は最大値取得に固定されています。min・sumなど別の集約を使う場合は、上のように `process` と汎用版の `query` で集約します。
HLDのパス区間は通過順には並ばないため、min・max・sumなど可換な集約に使用します。
また、KACTLのHLDのDFSは再帰なので、非常に深い木ではスタック容量に注意してください。

## Graph

- [全方位木DP](graph/rerooting.hpp)
- [木のEuler Tour](graph/euler_tour.hpp)

## Math

- [Mod（最小構成）](math/modint.hpp)
- [ModInt（演算子・入出力付き）](math/modint_manyfunc.hpp)
- [二項係数](math/combination.hpp)
- [形式的冪級数（FPS）](math/fps.hpp)：mod 998244353、`vector<Mod>` を継承した `fps` 型

### FPS

`Mod` を定義した後に `#include "library/math/fps.hpp"` を追加します。
現在の `modint.hpp` の `Mod` をそのまま使えます。FPSヘッダはModを定義・includeしないため、解答内に既にModがある場合にも使えます。

```cpp
fps f = {1, 2, 3}; // 1 + 2x + 3x^2
auto product = f * f;   // 全係数を返す
auto inverse = f.inv(10); // 1/f の x^0〜x^9
auto quotient = f.div(f, 10); // FPSの割り算（多項式の商ではない）
auto logarithm = f.log(10); // f[0] == 1 が必要
auto restored = logarithm.exp(10); // 入力の定数項は0
auto power = f.pow(3, 10); // f^3 の先頭10項
auto d = f.derivative();
auto integral = f.integral(); // 積分定数は0
auto truncated = f.prefix(10); // 切り詰め / 0埋め
cout << inverse[4].x << '\n';
```

係数は低次から並べます。`inv` / `div` の分母の定数項は非ゼロ、`pow` の指数は非負である必要があります。`f.pow(0, n)` は零級数でも1を返します。
`prefix` / `inv` / `div` / `log` / `exp` / `pow` はちょうど指定項数を返します。
積とNewton法を使う演算は O(n log n)、微分・積分は O(n) です。
modは998244353専用です。中間の積を含めてNTT長が2^23以下である必要があります。
`fps f(n)` と `f.resize(n)` は0埋めに対応しています。添字アクセスや反復などは `vector<Mod>` と同じで、演算結果も `fps` です。`Mod` の定義より後にincludeしてください。

## Misc

- [入出力の早見表](misc/io.md)：`ws`・`ignore`・`getline`・文字列の分解・出力書式
- [デバッグ出力](misc/debug.hpp)
- [乱数](misc/random.hpp)
- [2進文字列](misc/to_binary_string.hpp)

### priority_queueに比較関数を渡す

比較関数の型を3番目のテンプレート引数に、関数オブジェクトをコンストラクタに渡します。
次は `{コスト, 頂点番号}` をコストの小さい順、同じコストなら頂点番号の小さい順に取り出す例です。

```cpp
using P = pair<ll, int>;
auto comp = [](const P& a, const P& b) {
    if (a.first != b.first) return a.first > b.first;
    return a.second > b.second;
};
priority_queue<P, vector<P>, decltype(comp)> pq(comp);
```

`decltype(comp)` が比較関数の型、最後の `(comp)` が実際に渡す比較関数です。
`comp(a, b)` がtrueなら、aはbより優先度が低いと判断されます。小さい値を先に取り出す場合は `>`、大きい値を先に取り出す場合は `<` を返します。
通常のpairの辞書順なら、上の例は `priority_queue<P, vector<P>, greater<P>> pq;` と同じ動作です。

比較用のstructを定義して渡す書き方もできます。

```cpp
struct Comp {
    bool operator()(const P& a, const P& b) const {
        return a.first > b.first;
    }
};
priority_queue<P, vector<P>, Comp> pq;
```

このstructの例はコストだけを比較します。同じコストの要素の取り出し順は指定していません。

転倒数には座標圧縮、二項係数にはModIntなどの剰余演算の型も必要です。コピペ時は依存する実装も取り込み、ローカルヘッダへの `#include` を取り除いてください。
