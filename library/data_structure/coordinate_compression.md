# 座標圧縮の書き方

値の大小関係を保ったまま、異なる値に `0, 1, ..., K-1` の番号を付けます。同じ値には同じ番号が付きます。
例：`{100, 20, 100, 50}` → `{2, 0, 2, 1}`。

以下は `template.cpp` の型定義・マクロを使います。

## 基本：コピーしてsort → unique → lower_bound

```cpp
vl a = {100, 20, 100, 50};
vl xs = a;
sort(all(xs));
xs.erase(unique(all(xs)), xs.end()); // xs = {20, 50, 100}

vi b(sz(a));
rep(i,sz(a)) {
    b[i] = lower_bound(all(xs), a[i]) - xs.begin();
}
// b = {2, 0, 2, 1}
```

- `xs`：異なる元の値を小さい順に並べた配列
- `b[i]`：`a[i]` の圧縮後の番号（0始まり）
- `sz(xs)`：異なる値の個数K。圧縮後の値を添字にする配列・セグ木のサイズ
- `xs[b[i]]`：圧縮前の値 `a[i]` に戻す

`unique` は重複部分を実際に削除しないので、`erase` まで書きます。
計算量は全体で O(N log N)、追加メモリは O(N) です。

**値の差・距離は保たれません。** 例えば元の `100 - 20 = 80` に対し、圧縮後は `2 - 0 = 2` です。
差や距離を計算するときは `xs` の元の値を使います。

## 複数の配列を同じ基準で圧縮する

```cpp
vl xs = a;
xs.insert(xs.end(), all(b)); // aとbの値を全部集める
sort(all(xs));
xs.erase(unique(all(xs)), xs.end());

auto id = [&](ll x) {
    return int(lower_bound(all(xs), x) - xs.begin());
};

// a[i]にもb[j]にも同じidを使う
// 例：ca[i] = id(a[i]); cb[j] = id(b[j]);
```

同じ番号で比較したい配列は、値を全部集めてから圧縮します。
上の `id(x)` を圧縮後の番号として使うのは、`xs` に登録済みの値に対してです。

## 既存のCompressライブラリを使う場合

[coordinate_compression.hpp](coordinate_compression.hpp) の `Compress` でも同じ操作ができます。

```cpp
Compress<ll> c(a);           // 構築
vi b = c.compress(a);       // 配列全体を圧縮
int p = c.rank(a[0]);       // 登録済みの値1つを圧縮
ll x = c[b[0]];             // 元の値に戻す
int k = c.size();           // 異なる値の個数
```

複数配列なら、すべて追加してから1回だけ `build()` します。

```cpp
Compress<ll> c;
c.add(a);
c.add(b);
c.build();
vi ca = c.compress(a);
vi cb = c.compress(b);
```
