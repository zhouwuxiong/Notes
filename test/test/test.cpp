#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

int main()
{
    ifstream statm_file("/proc/" + to_string(getpid()) + "/statm");
    unsigned long size;
    statm_file >> size;
    statm_file.close();

    cout << "Current process's memory usage: " << size * getpagesize() / 1024 << " Kb" < < < < endl;

    return 0;
}