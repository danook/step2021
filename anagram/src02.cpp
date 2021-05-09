#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <array>
#include <algorithm>
using namespace std;

// arrayのsum(=score)降順に辞書をソート
auto is_former = [](array<int,26> l, array<int,26> r){
    int lsum=0, rsum=0;
    for(int i=0; i<26; ++i){
        lsum += l[i];
        rsum += r[i];
    }
    return lsum > rsum;
};

// 辞書
multimap<array<int, 26>, string, decltype(is_former)> dict(is_former);    // ソート後の辞書

//各アルファベットのスコア
const int SCORE[26] = {
    1,3,2,2,1,3,3,1,1,4,4,2,2,1,1,3,4,1,1,1,2,3,3,4,3,4
};

void solve(string str){

    array<int,26> arr = {0};    // 出現頻度
    for(char c: str) arr[c-'a'] += SCORE[c-'a'];

    // スコアが大きい単語から調べる
    for(auto word: dict){
        bool flg = true;
        for(int i = 0; i < 26; ++i){
            if (arr[i] < word.first[i]){
                flg = false;
                break;
            }
        }
        if (flg) {
            cout << word.second << endl;
            return;
        }
    }
    cout << "NOT FOUND" << endl;
    return;
}

int main(){

    string filepath = "input_data\\words.txt";   //辞書のファイルパス
    
    // 辞書ファイルを開く
    ifstream ifs(filepath);
    if(!ifs){
        cout << "File not opened." << endl;
        system("pause");
        return 1;
    }

    // 辞書を読みだす
    string str;
    while(getline(ifs, str)){
        array<int,26> arr = {0};
        // 出現頻度をアルファベットのスコアで重みづけ
        for(char c: str) arr[c-'a'] += SCORE[c-'a'];
        dict.insert(make_pair(arr, str));
    }

    // main
    string tmp;
    while(cin >> tmp) solve(tmp);

    return 0;
}
