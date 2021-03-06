# 宿題2について

## ハッシュテーブルはメモリを多く使用するから
二分探索木の場合、一つ要素を追加するたびに動的にメモリを確保できるが、ハッシュテーブルでは、初めにある程度まとまった量のメモリを確保するため、無駄が出る。とくにNが想定よりも小さかった時には必要よりも遥かに多いメモリが使われる。

## 入力するデータの種類に応じて、適切なハッシュ関数を設計する必要があるから
たとえば、住所(長い文字列、長さは異なる)をキーに持つ場合と郵便番号(すべて7桁の数字列)をキーに持つ場合では適切なハッシュ関数は異なる。後者の場合は、郵便番号を10進数に変換したものをそのままハッシュ関数の返り値とできるが、前者ではそれはできない。適したハッシュ関数でないと、ハッシュ値の重複が多くなってしまったりして、効率が悪いと考えられる。

## データの数が想定より極端に大きかった際に、時間をかけて組み直す必要があるため
多少の超過であれば超過した部分だけ新たにハッシュテーブルを付け加えれば済むが、大幅に超過した場合には再ハッシュする必要があり、これにはO(N)時間(Nはデータ数)がかかる。二分探索木の場合、要素を一つ追加するたびにO(1)時間で回転させてバランスを取ればこのような組み換えの必要はない。

## 上記の組み直しは自動化が難しいため
どの段階で組みなおしをするか、というのはケースバイケース(データの増える速度、ハッシュ値の偏り具合などに依存する)で、プログラムの中にその判定を組み込んで自動化するのは難しいのではないか。