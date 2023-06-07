/*
 * 有锁与无锁性能对比测试
 *      g++ cas.cc -std=c++11 -pthread  -o build/cas
 */

#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>
#include <mutex>

using namespace std;

// 无锁多线程入栈
template <typename T>
class LockFreeStack
{
private:
    struct node
    {
        T data;
        node *next;
        node(T const &data_) : data(data_) {}
    };
    std::atomic<node *> head;

public:
    void push(T const &data)
    {
        node *const new_node = new node(data);
        new_node->next = head.load(); // 这里只是用 Head 的地址占位，

        // 随机延迟0-10微秒，模拟实际业务中的线程竞争中head发生变化的情况
        int num = rand() % 10;
        std::this_thread::sleep_for(std::chrono::microseconds(num));

        // push
        // 如果head!=new_node->next，会执行new_node->next = head.load()操作并返回false；while循环后再次判断，如果此时head=new_node->next，则执行head.store(new_node)并返回true结束循环

        while (!head.compare_exchange_strong(new_node->next, new_node))
            ;
        // std::cout << "push [" << data << "] done!  " << clock()*1000.0/CLOCKS_PER_SEC << std::endl;
    }
};

// 有锁多线程入栈
template <typename T>
class LockStack
{
private:
    struct node
    {
        T data;
        node *next;
        node(T const &data_) : data(data_) {}
    };
    node *head;
    mutex mtx; // 全局互斥锁

public:
    void push(T const &data)
    {
        lock_guard<mutex> lock(mtx); // 持有互斥锁

        // 随机延迟0-10微秒，模拟实际业务中的线程竞争导致的取锁延迟
        int num = rand() % 10;
        std::this_thread::sleep_for(std::chrono::microseconds(num));

        // push
        node *const new_node = new node(data);
        new_node->next = head;
        head = new_node;
        // std::cout << "push [" << data << "] done#  " << clock()*1000.0/CLOCKS_PER_SEC << std::endl;
    }
};

LockFreeStack<string> lf;
void push_lock_free_stack(const int i)
{
    lf.push(to_string(i));
}

LockStack<string> ls;
void push_lock_stack(const int i)
{
    ls.push(to_string(i));
}

int main(int argc, char *argv[])
{
    // LockFreeStack<string> lf;
    // lf.push("a");
    // LockStack<string> ls;
    // ls.push("b");

    // 无锁多线程入栈
    thread thm[1000];
    for (int i = 0; i < 1000; i++)
    {
        thm[i] = thread(push_lock_free_stack, i + 1);
    }
    clock_t start_time = clock();
    for (int i = 0; i < 1000; i++)
    {
        thm[i].join();
    }
    clock_t cost_time = (clock() - start_time) * 1000.0 / CLOCKS_PER_SEC;
    cout << "push_lock_free_stack cost: " << cost_time << "ms" << endl;

    // 有锁多线程入栈
    thread thl[1000];
    for (int i = 0; i < 1000; i++)
    {
        thl[i] = thread(push_lock_stack, i + 1); //
    }
    start_time = clock();
    for (int i = 0; i < 1000; i++)
    {
        thl[i].join();
    }
    cost_time = (clock() - start_time) * 1000.0 / CLOCKS_PER_SEC;
    cout << "push_lock_stack cost: " << cost_time << "ms" << endl;

    return 0;
}
