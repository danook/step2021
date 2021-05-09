#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

multimap<string, string> dict;    // ソート後の辞書

void solve(string str){
    sort(str.begin(), str.end());
    // 二分探索
    auto itr1 = dict.lower_bound(str);
    auto itr2 = dict.upper_bound(str);
    if (itr1 == itr2) cout << "NOT FOUND" << endl;
    else {
        for(auto itr = itr1; itr != itr2; ++itr){
            cout << (*itr).second << endl;
        }
    }
}

int main(){

    string filepath = "input_data\\words.txt";   //辞書のファイルパス
    
    // ファイルを開く
    ifstream ifs(filepath);
    if(!ifs){
        cout << "File not opened." << endl;
        system("pause");
        return 1;
    }

    // 辞書を読みだす
    string str;
    while(getline(ifs, str)){
        string newstr = str;
        // ソートしてdictに挿入
        sort(newstr.begin(), newstr.end());
        dict.insert(make_pair(newstr, str));
    }

    // main
    string tmp;
    while(cin >> tmp) solve(tmp);

    return 0;
}
