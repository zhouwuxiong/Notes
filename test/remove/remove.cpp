#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    remove(a.begin(), a.end(), 2);

    for (auto it : a)
        cout << it << " ";

    vector<int> b;
    remove_copy_if(a.begin(), a.end(), b.begin(), 2);

    for (auto it : b)
        cout << it << " ";
}