#include <iostream>

using namespace std;

void ctrlc_handler(int s)
{
    cout << __func__ << endl;
    exit(1);
}

void TerminateProcess()
{
    cout << __func__ << endl;
    std::abort(); // 终止程序
}

int main()
{
    std::abort(); // 终止程序
}
