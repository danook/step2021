# Google STEP Week7 Homework
## 概要

## 実行方法
solver.cpp, utils.cpp, utils.hppと入出力ファイルは同一ディレクトリ内に置く必要があります。
```
cd week7\tsp\google-step-tsp
g++ -o solver.exe -O3 utils.cpp solver.cpp
```
でコンパイルをしたのち、以下のコマンドでexeファイルを実行してください。
```
solver.exe (input_file_name) (output_file_name)
```

## 結果
Score(greedy): 3418.1
Score(two-opt): 3418.1
Score(final): 3291.62
Score(greedy): 3832.29
Score(two-opt): 3832.29
Score(final): 3778.72
Score(greedy): 5449.44
Score(two-opt): 5069.5
Score(final): 4494.42
Score(greedy): 10519.2
Score(two-opt): 8965.71
Score(final): 8330.94
Score(greedy): 12684.1
Score(two-opt): 11385.2
Score(final): 10871.7
Score(greedy): 25331.8
Score(two-opt): 21653
Score(final): 20699
Score(greedy): 49892
Score(two-opt): 42847.1
Score(final): 40870.9
start_temp = 2.5
end_temp = 0.1
tempは線型
two-opt 10 sec.
move 1800 sec.
で40802.5

output_6.csvに保存してある

あとは初期値を改善すればもう少し行くかもしれない
greedy + two_optを何通りか試すとか
two_optは10s要らない
start_tempはもう少し高めでも良さげ

g++ -o solver.exe -O3 utils.cpp solver.cpp
solver.exe input_6.csv output_temp.csv
