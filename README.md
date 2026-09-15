# ICPC Templates & Library

チームで共有する競技プログラミング用のC++20テンプレートとライブラリです。
必要なコードをコピペして使う想定です。テンプレートの調整や実行環境は各自の自由です。

## 内容

- `template.cpp`: テンプレート
- [library/](library/README.md): 自作ライブラリ
- `kactl/`: 参照用のKACTL submodule

練習コード（`problems/`）、実行・検証ツール（`scripts/`、`stress/`、`run`、`judge` など）、エディタ設定はGit管理外です。
個人用の `template.local.cpp` と `README.local.md` も共有対象から除外しています。

## KACTLの取得

```sh
git submodule update --init --recursive
```
