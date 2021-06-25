#include <iostream>
using namespace std;

int main()
{
    int i = 0;
    for (int i = 0; i < 10; ++i)
    {
        if (i == 2)
        {
            for (; i < 7; ++i)
            {
                cout << i << "*" << endl;
            }
        }
        cout << i << endl;
    }
}