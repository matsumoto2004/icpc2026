# C++ 入出力の早見表

`#include <bits/stdc++.h>` と `using namespace std;` を使う想定です。
機能名を忘れたら、下の「やりたいこと」から探します。

| やりたいこと | 書き方 |
| --- | --- |
| 空白区切りで読む | `cin >> x;` |
| 空白を含めて1行読む | `getline(cin, s);` |
| 先頭の空白・改行を飛ばして1行読む | `getline(cin >> ws, s);` |
| 今の行の残りを捨てる | `cin.ignore(numeric_limits<streamsize>::max(), '\n');` |
| 1文字だけ読み捨てる | `cin.ignore();` |
| 空白・改行も含めて1文字読む | `cin.get(c);` |
| 次の文字を消費せずに見る | `int c = cin.peek();`（EOFの場合もある） |
| 行や文字列を空白区切りで分解する | `istringstream ss(s);` と `ss >> x` |
| 指定した文字で区切って読む | `getline(ss, s, ',');` |
| 入力が終わるまで読む | `while (cin >> x) { ... }` |
| 小数点以下k桁で出す | `cout << fixed << setprecision(k) << x;` |
| 5桁にゼロ埋めして出す | `cout << setfill('0') << setw(5) << x;` |

## 数値の後に文字列を1行ずつ読む

```cpp
int n;
cin >> n;
vector<string> a(n);
for (auto& s : a) getline(cin >> ws, s);
```

`cin >> n` は数値の直後の改行を残します。`ws` はその改行を含む先頭の空白文字をすべて飛ばします。
行の途中の空白は保持しますが、空行と行頭の空白は読み飛ばします。

空行・行頭の空白もデータなら、数値を読んだ行の残りだけを捨ててから読みます。

```cpp
cin >> n;
cin.ignore(numeric_limits<streamsize>::max(), '\n');
for (auto& s : a) getline(cin, s);
```

`getline` は区切り文字（通常は改行）も消費しますが、文字列には含めません。
`cin.ignore()` の引数省略は1文字だけです。改行までまとめて捨てる場合は上の形を使います。

### wsやignoreの書き方を忘れた場合

「最初の行にn、その後に文字列がn行」という形式なら、`getline` で最初の行の残りを1回読み捨てれば同じことができます。

```cpp
int n;
cin >> n;
string s;
getline(cin, s); // nがあった行の残りを捨てる
for (int i = 0; i < n; ++i) {
    getline(cin, s);
    // sを処理する。空行・行頭の空白も保持される。
}
```

## 1行に何個あるか分からない数値を読む

```cpp
string s;
getline(cin, s); // 例: "10 20 30"
istringstream ss(s);
long long x;
while (ss >> x) {
    // xを処理する
}
```

`ss` は、その文字列の中だけを読む入力ストリームです。

## カンマなどで区切る

```cpp
istringstream ss("10,20,30");
string s;
while (getline(ss, s, ',')) {
    long long x = stoll(s);
    // xを処理する
}
```

`getline` の第3引数で区切り文字を指定できます。この例は各区間が整数である入力用です。

## 入力の終わりまで読む

```cpp
long long x;
while (cin >> x) {
    // xを処理する
}
```

行単位なら `while (getline(cin, s))`。読み取りが成功したかをループ条件にします。
`while (!cin.eof())` では、読み取りに失敗した回にも本体を実行することがあります。

## 出力の指定

```cpp
cout << fixed << setprecision(10) << x << '\n';
cout << setfill('0') << setw(5) << 42 << '\n'; // 00042
```

`fixed` がない `setprecision(k)` は、有効数字k桁の指定です。
`fixed`・`setprecision`・`setfill` の設定は後の出力にも残ります。`setw` は次の項目にだけ適用されます。

`'\n'` は改行、`endl` は改行に加えてflush（即時出力）を行います。

## 入出力高速化

`main` の先頭に置きます。ルートの `template.cpp` には追加済みです。

```cpp
ios::sync_with_stdio(false);
cin.tie(nullptr);
```
