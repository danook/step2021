# STEP Week4 Homework
## 概要
Wikipediaのページ間のリンクの情報から以下のものを計算し表示します。
1. "Google"から"渋谷"への最短パス
2. "Google"と連結していないページの数
3. "Google"と連結している中で最も遠いページと、"Google"からそのページへのパス

## 準備
[wikipedia_data.zip](https://drive.google.com/file/d/1zqtjSb-ZoR4rzVUWZrjNSES5GKJhYmmH/view?usp=sharing)をダウンロードして解凍し、以下のようなディレクトリ構成にしてください。
```
step_wikipedia_graph
├── data
│   ├── graph_small.png
│   ├── links_small.txt
│   ├── links.txt
│   ├── pages_small.txt
│   └── pages.txt
├── README.md
├── path_from_google.py
└── queue_for_pages.py
```

## 実行方法
以下のコマンドで実行してください。
```
python3 path_from_google.py
```
※ テスト環境: Python 3.9.1 64-bit  
※ 結果の出力までに数分ほどかかります。

## 出力結果
以下の形式で出力されます。
```
Path from "Google" to "渋谷":
(Googleから渋谷へのパス)

(Googleから連結していないページ数) in 1483277 pages are not accessible from the page "Google".

One of the farthest page from "Google": (Googleから最も遠いページのタイトル)
Path from "Google" to "(Googleから最も遠いページのタイトル)":
(Googleからそのページへのパス)
```