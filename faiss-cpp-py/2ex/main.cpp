#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1e5 + 10;

int x, y;

int add(int& a, int& b)
{
    a ++, b ++;
    cout << a + b << endl;
    return a + b;
}

int main()
{
    scanf("%d%d", &x, &y);
    cout << x << " " << y << endl;
    add(x, y);
    int m = add(x, y);
    cout << m << endl;
    return 0;
}
