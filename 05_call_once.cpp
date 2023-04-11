// 05_call_once.cpp

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <string>
#include <mutex>

void init()
{
    std::cout << "Initing ..." << std::endl;
    // Do something ....
}

void worker(std::once_flag* flag)
{
    std::call_once(*flag, init);
}

int main()
{
    std::once_flag flag;

    std::thread t1(worker, &flag);
    std::thread t2(worker, &flag);
    std::thread t3(worker, &flag);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}