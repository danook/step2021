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