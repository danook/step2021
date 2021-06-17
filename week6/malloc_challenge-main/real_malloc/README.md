# STEP Week6 Homework 
## 概要
malloc.cの実装をfirst-fitからbest-fit, worst-fitに変えて、実行時間とメモリ使用率を比較した。また、malloc.cを改善したものをimproved_malloc.cとして実装し、それについても実行時間とメモリ使用率を計測した。

## 実行方法
### malloc.c
以下のコマンドを実行する。
```
gcc -O3 -o malloc_challenge.bin main.c malloc.c simple_malloc.c -lm
./malloc_challenge.bin
```
### improved_malloc.c
以下のコマンドを実行する。
```
gcc -O3 -o malloc_challenge.bin main.c improved_malloc.c simple_malloc.c -lm
./malloc_challenge.bin
```

## best-fit, worst-fitの実装
malloc.cの128~132行目のfirst-fitを、134~156行目のbest-fitに書き換えた。領域を確保する際に、すべての空きスロットを見て、確保サイズより大きい中で最もサイズの小さい領域を探している。150行目の不等号"<"を">"にすることでworst-fitとなる。

## improved_malloc.cにおける工夫
improved_malloc.cでは、best-fitを用いることに加え、さらに以下のような工夫をした。

### 隣接した空スロットの連結
my_add_to_free_list()が呼ばれた際に、新しくfree listに加えたスロットと隣接(右または左で)する空スロットがないか探し、ある場合はそれらを連結して1つのスロットとすることでメモリ使用率と時間を改善することを目指した。連結する際には、左側のスロットのサイズを右側のスロットのサイズ分だけ増やし、右側のスロットをfree listから取り除くという処理を行っている。

### metadataの双方向リスト化
空スロット連結を実装するにあたり、片方向リストでの実装を難しく感じたので、metadataを双方向リストとした。(片方向リストの場合、my_remove_from_free_listにprevを引数として渡さなくてはならず、improved_malloc.cの105~128行目でmetadata_to_mergeに対するprevを保持して…というあたりで良く分からなくなった)


## 結果
**first-fit**: 元々のsimple_malloc.cをそのまま写した場合  
**best-fit**: malloc.cの128~132行目のfirst-fitを、134~156行目のbest-fitに書き換えた場合  
**worst-fit**: best-fitの不等号を逆にしてworst-fitとした場合  
**bidirectional list**: improved_malloc.cで、空スロット連結処理部分(105~128行目)を削除した場合  
**improved**: improved_malloc.c(best-fit＋双方向リスト化＋空スロット連結)  

実行環境: Ubuntu 20.04.2 LTS (Windows 10 WSL)

|   |Challenge 1|Challenge 2|Challenge 3|Challenge 4|Challenge 5|
| --- | --- | --- | --- | --- | --- |
| first fit | Time: 20 ms => 18 ms<br>Utilization: 70% => 70% | Time: 21 ms => 16 ms<br>Utilization: 40% => 39% | Time: 451 ms => 424 ms<br>Utilization: 7% => 7% | Time: 39131 ms => 39779 ms<br>Utilization: 16% => 15% | Time: 31796 ms => 32518 ms<br>Utilization: 15% => 14% |
| best fit | Time: 29 ms => 2909 ms<br>Utilization: 70% => 70% | Time: 27 ms => 1614 ms<br>Utilization: 40% => 39% | Time: 453 ms => 1671 ms<br>Utilization: 7% => 50% | Time: 43596 ms => 22357 ms<br>Utilization: 16% => 71% | Time: 36616 ms => 12737 ms<br>Utilization: 15% => 71% |
| worst fit | Time: 14 ms => 1823 ms<br>Utilization: 70% => 70% | Time: 10 ms => 1058 ms<br>Utilization: 40% => 39% | Time: 413 ms => 127568 ms<br>Utilization: 7% => 3% | Time: 29951 ms => 827190 ms<br>Utilization: 16% => 6% | Time: 27168 ms => 712426 ms<br>Utilization: 15% => 6% |
| bidirectional list | Time: 13 ms => 1831 ms<br>Utilization: 70% => 65% | Time: 14 ms => 1012 ms<br>Utilization: 40% => 31% | Time: 303 ms => 1442 ms<br>Utilization: 7% => 43% | Time: 36137 ms => 13731 ms<br>Utilization: 16% => 70% | Time: 27846 ms => 8253 ms<br>Utilization: 15% => 70% |
| **improved** | Time: 14 ms => 1476 ms<br>Utilization: 70% => 65% | Time: 32 ms => 774 ms<br>Utilization: 40% => 31%| Time: 270 ms => 947 ms<br>Utilization: 7% => 38% | Time: 34885 ms => 4196 ms<br>Utilization: 16% => 74% | Time: 28744 ms => 3530 ms<br>Utilization: 15% => 73% |
   
* Challenge 1~3の時間については、first fitが最も速い結果になった。この2つはfirst-fitでもメモリ効率が十分良いので、best-fitで「もっともサイズの小さいスロットを探す」ための時間がかえってボトルネックとなってしまったのかもしれない。メモリ効率はいずれもほとんど変化がなかった。
* 一方Challenge4,5については、実行時間、メモリ効率ともにbest-fit, first-fit, worst-fitの順に良い結果となった。
* metadataを双方向リストにするとメモリ効率は悪くなった。これはmetadataにより多くの領域を用いるようになったためだと思われる。しかし実行時間はやや短くなった。
* 空スロットを連結してもメモリ効率はほとんど改善しなかった…。しかし時間はかなり短縮することができた。

## その他質問したいことなど
* testをどう書けばよいのか分からず結局書いていません…。
* malloc.c 185行目でなぜ突然char*が出てくるのか分からないです。
* あとmy_finalize()を何のために使うのかもよくわからないです。

