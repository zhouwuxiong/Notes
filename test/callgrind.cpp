#include <stdio.h>
#include <unistd.h>
#include <valgrind/callgrind.h>

class A
{
public:
    void perf1()
    {
        unsigned int i = 0;
        while (i < 200)
        {
            int b = i++;
        }
        usleep(10);
    }

    void perf()
    {
        int i = 0;
        while (i++ < 100000)
        {
            int b = i;
        }
        i = 0;
        while (i++ < 10)
        {
            perf1();
        }
    }
};

int main()
{
    A a;
    unsigned int i = 0;
    while (i++ < 10000)
    {
        CALLGRIND_TOGGLE_COLLECT;
        a.perf();
        CALLGRIND_TOGGLE_COLLECT;
    }
    // CALLGRIND_DUMP_STATS;
    CALLGRIND_DUMP_STATS_AT("test");
    return 0;
}
