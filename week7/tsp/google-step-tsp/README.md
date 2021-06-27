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

焼きなまし法を導入したらもっと良くなりそうな気はしている
10分間の探索で40855.6いった！！！

g++ -o solver.exe -O3 utils.cpp solver.cpp
solver.exe input_6.csv output_temp.csv