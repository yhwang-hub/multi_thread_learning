// 06_naive_multithread.cpp

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
static const int MAX = 10e8;
static double sum = 0;


void worker(int min, int max)
{
    for (int i = min; i <= max ; i++)
    {
        /* code */
        sum += std::sqrt(i);
    }
}

void serial_task(int min, int max)
{
    auto start_time = std::chrono::steady_clock::now();
    sum = 0;
    worker(min, max);
    auto end_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Serial task finish, " << ms << " ms consumed, Result: " << sum << std::endl;
}

void concurrent_task(int min, int max)
{
    auto start_time = std::chrono::steady_clock::now();
    
    unsigned concurrent_count = std::thread::hardware_concurrency();
    std::cout << "hardware_concurrency: " << concurrent_count << std::endl;
    std::vector<std::thread> threads;
    // min = 0;
    sum = 0;
    for (int t = 0; t < concurrent_count; t++)
    {
        /* code */
        int range = max / concurrent_count * (t +1);
        threads.push_back(std::thread(worker, min, range));
        min = range + 1;
    }
    for (auto& t : threads)
    {
        /* code */
        t.join();
    }

    auto end_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time
    ).count();
    std::cout << "Concurrent task finish, " << ms << " ms consumed, Result: " << sum << std::endl;
}

int main()
{
    serial_task(0, MAX);
    concurrent_task(00, MAX);

    return 0;
}