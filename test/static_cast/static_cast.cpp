#include <iostream>

using namespace std;

int main()
{
    int x = 1, y = 1, z = 1;
    int16 a = (static_cast<unsigned long>(x) << 4 * 8 | static_cast<unsigned long>(y) << 8 | static_cast<unsigned long>(z));
    cout << a;
}