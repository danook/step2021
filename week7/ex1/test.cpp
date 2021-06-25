#include <iostream>
#define SIZE 500
using namespace std;
int main()
{
    int a[SIZE][SIZE];
    int b[SIZE][SIZE];
    int c[SIZE][SIZE];
    a[99][99] = 1;
    cout << "Suceed" << endl;
    return 0;
}

// スタック領域をあふれてしまっているらしい。Ubuntuならうまくいくかな・・。