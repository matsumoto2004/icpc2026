# ICPC Templates & Library

チームで共有する競技プログラミング用のC++20テンプレートとライブラリです。
必要なコードをコピペして使う想定です。テンプレートの調整や実行環境は各自の自由です。

## 参考資料

- [構文解析 Howto（draftcode）](https://gist.github.com/draftcode/1357281): 再帰下降構文解析の解説とC++の実装例

## 内容

- `template.cpp`: テンプレート
- [library/](library/README.md): 自作ライブラリ
- `kactl/`: 参照用のKACTL submodule

練習コード（`problems/`）、実行・検証ツール（`scripts/`、`stress/`、`run`、`judge` など）、エディタ設定はGit管理外です。
個人用の `template.local.cpp` と `README.local.md` も共有対象から除外しています。
`.gitignore` はルートの公開対象を限定しています。自分で解いた問題のコードは `problems/` に置いてください。ルートに新しく置いたファイルも、明示的に公開対象へ追加しない限りGit管理外になります。

## AtCoderでの使用と出典

公開先: https://github.com/matsumoto2004/icpc2026

このリポジトリのテンプレート・ライブラリをコピーして提出する際は、コード内の次のコメントを残してください。READMEへの記載だけでは、提出コードへの参照URLの添付にはなりません。

```cpp
// Source: https://github.com/matsumoto2004/icpc2026
```

[AtCoder生成AI対策ルール（20261014版）](https://info.atcoder.jp/entry/llm-rules-ja)では、ABC・ARC・AGCで生成AIを用いて作成したコードを使用する場合、**使用する版をコンテスト開始前にWeb上で公開し、提出コードに参照URLを含める**必要があります。非公開リポジトリやローカルのcommitだけでは事前公開になりません。更新したコードは、使用するコンテストの開始前に公開リポジトリへpushしてください。バージョンを特定したい場合は、公開済みのcommitへのURLも使えます。

自力で解いた問題の解答は公開対象に含めません。AIを使わず自分で書いたコードは、この生成AIコードの事前公開要件の対象外です。ただし、自力で解いた問題でも、このリポジトリのAI作成コードを含むテンプレート・ライブラリを流用した場合は、その部分の出典コメントを残してください。解答全体をこのリポジトリで公開する必要はありません。

`kactl/` の原本の出典は [KACTL](https://github.com/kth-competitive-programming/kactl) です。原本の著者・ライセンス・出典コメントも保持してください。submodule内のローカル編集は、このリポジトリへのpushだけでは公開されません。追加の区間探索関数は [library/README.md](library/README.md) に掲載しています。

## KACTLの取得

```sh
git submodule update --init --recursive
```
