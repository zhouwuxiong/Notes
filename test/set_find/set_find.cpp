#include <iostream>
#include <set>

using namespace std;

int main()
{
    set<int *> test;
    for (int i = 0; i < 10; i++)
        test.insert(new int(0));
    for (auto iter : test)
        cout << iter << endl;
}