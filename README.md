# ICPC Templates & Library

チームで共有する競技プログラミング用のC++20テンプレートとライブラリです。個人の練習コード・実行環境はGit管理外に置きます。

## 共有するもの

- `template.cpp`: 共通の初期テンプレート
- `library/`: 再利用するC++コード、使用例、互換性テスト
- `debug.hpp`: `library/misc/debug.hpp` を読み込む互換用ヘッダ
- `kactl/`: 参照用のKACTL submodule（取得先は `.gitmodules` に記載）
- `README.md`、`.gitignore` などの共有用ドキュメント・設定

## 各自のテンプレート

最初に共通テンプレートをコピーします。

```sh
cp template.cpp template.local.cpp
```

`template.local.cpp` はGit管理外です。マクロ、型の別名、入出力設定などを各自の好みに合わせて変更してください。共通テンプレートの更新は自動では反映されないため、必要な変更を個別に取り込みます。

新しい解答は個人用テンプレートから作成します。

```sh
mkdir -p problems/other/example
cp template.local.cpp problems/other/example/A.cpp
```

共通テンプレートでは `rep(i, n)` と `rep(i, a, b)` の両方を使用できます。`main` は空にしてあります。

## ライブラリの使用

解答から必要なヘッダをincludeします。

```cpp
#include "library/graph/euler_tour.hpp"
```

コンパイル時にリポジトリルートをinclude pathへ追加してください。以下はリポジトリルートでの実行例です。C++20と `<bits/stdc++.h>` が使えるGCC環境を想定しています。`g++` は各自のコンパイラ名・パスに置き換えてください。

```sh
mkdir -p .build
g++ -std=c++20 -O2 -Wall -Wextra -I. problems/other/example/A.cpp -o .build/A
./.build/A
```

デバッグ出力を有効にする場合は `-DLOCAL` を追加します。各ライブラリのAPI、使用条件、使用例は [library/README.md](library/README.md) を参照してください。

提出先でこのリポジトリのヘッダを参照できない場合は、依存するコードも含めて解答へコピーしてください。

## 個人用ファイル

次のファイル・ディレクトリは `.gitignore` で共有対象から除外しています。

- `problems/`: 練習用の解答・問題データ
- `scripts/`、`run`、`judge`、`stress-test`、`stress/`: 個人の実行・検証ツール
- `.vscode/`、`.idea/`: エディタ設定
- `template.local.cpp`: 個人用テンプレート
- `README.local.md`: 個人の環境・操作メモ
- `.build/`、`input.txt`、`output.txt` など: ビルド成果物・ローカル入力

これらはcloneしても含まれません。実行ツールやエディタ設定は各自で用意してください。

## 共有コードの更新

ライブラリの修正・追加は `library/` に反映します。個人のテンプレートにあるマクロや型の別名に依存させず、ヘッダ単体でincludeできる形にしてください。区間の端点、使用条件、計算量を明記し、検証してから共有します。

チーム全体で採用したいテンプレートの変更は `template.cpp` に反映します。

## KACTL

KACTLを参照する場合や互換性テストを実行する場合は、submoduleを取得してください。

```sh
git submodule update --init --recursive
```

KACTLは参照用として扱い、必要なコードを理解・検証したうえで取り込みます。
