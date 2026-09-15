# C++ Library

必要な実装をコピペして、各自のコードに合わせて使ってください。使用条件や計算量は各ファイルのコメントを参照してください。

## Data Structure

- [座標圧縮](data_structure/coordinate_compression.hpp)
- [汎用Lazy Segment Tree](data_structure/generic_lazy_segment_tree.hpp)
- [転倒数](data_structure/inversion_count.hpp)

## Graph

- [全方位木DP](graph/rerooting.hpp)
- [木のEuler Tour](graph/euler_tour.hpp)

## Math

- [ModInt](math/modint.hpp)
- [二項係数](math/combination.hpp)

## Misc

- [デバッグ出力](misc/debug.hpp)
- [乱数](misc/random.hpp)
- [2進文字列](misc/to_binary_string.hpp)

転倒数には座標圧縮、二項係数にはModIntなどの剰余演算の型も必要です。コピペ時は依存する実装も取り込み、ローカルヘッダへの `#include` を取り除いてください。
