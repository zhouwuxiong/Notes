// constructing threads
#include <iostream> // std::cout
#include <atomic>   // std::atomic
#include <thread>   // std::thread
#include <vector>   // std::vector

#include <iterator>
#include <random>
#include <algorithm>

using namespace std;

// 伪随机，程序每次执行产生的相同的序列
static int pseudo_RandomInt(int min, int max)
{
    int d = max - min + 1;
    return int(((double)rand() / ((double)RAND_MAX + 1.0)) * d) + min;
}

// 真随机，程序每次执行产生的不同的序列
static int real_RandomInt(int min,int max){
    std::random_device rd;
    std::mt19937 g(rd());
    
    int d = max - min + 1;
    return int(((double)g() / ((double)RAND_MAX + 1.0)) * d) + min;
}

// 随机打乱 vector
static bool RandomVector(std::vector<int> &v)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);

    return true;
}

// 序列随机，短时间内会得到相同的随机结果，时间间隔较长时，得到的结果不一样的
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
    // // BoW
    // for (int i = 0; i < 10; i++)
    //     std::cout << (double)rand() << "," << (double)RAND_MAX + 1.0 << "," << RAND_MAX <<","<< RandomInt(0, 10) << std::endl;
    
    std::cout<<"pseudo-random: "<<endl;
    for (int i = 0; i < 10; i++)
        std::cout << pseudo_RandomInt(0, 10) <<" ";
    std::cout<< std::endl;

    std::cout<<"real-random"<<endl;
    for (int i = 0; i < 10; i++)
        cout<< real_RandomInt(0,10)<<" ";
    std::cout<<endl;

    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    RandomVector(v);
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    RandomInt_2();
}