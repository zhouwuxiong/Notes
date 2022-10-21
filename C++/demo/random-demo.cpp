// constructing threads
#include <iostream> // std::cout
#include <atomic>   // std::atomic
#include <thread>   // std::thread
#include <vector>   // std::vector

#include <iterator>
#include <random>
#include <algorithm>

using namespace std;

static bool RandomInt(int min, int max)
{
    int d = max - min + 1;
    return int(((double)rand() / ((double)RAND_MAX + 1.0)) * d) + min;

    return true;
}

static bool RandomVector(std::vector<int> &v)
{

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);

    return true;
}

static bool RandomInt_2()
{
    srand((unsigned)time(NULL));

    for (int i = 0; i < 5; i++)
        cout << rand() << '\t';
    cout << endl;

    return 0;
}

int main()
{
    // BoW
    for (int i = 0; i < 5; i++)
        std::cout << (double)rand() << "," << (double)RAND_MAX + 1.0 << "," << RAND_MAX << RandomInt(0, 10) << std::endl;

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    RandomVector(v);

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    RandomInt_2();
}