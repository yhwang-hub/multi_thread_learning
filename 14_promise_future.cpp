// 14_promise_future.cpp

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <set>
#include <condition_variable>
#include <future>

static const int MAX = 10e8;
// static double sum = 0;

double concurrent_worker(int min, int max)
{
    double sum = 0;
    for (int i = min; i <= max; i++)
    {
        sum += std::sqrt(i);
    }
    
    return sum;
}

void concurrent_task(int min, int max, std::promise<double>* result)
{
    std::vector<std::future<double>> results;

    unsigned concurrent_count = std::thread::hardware_concurrency();
    min = 0;
    for (int i = 0; i < concurrent_count; i++)
    {
        std::packaged_task<double(int, int)> task(concurrent_worker);
        results.push_back(task.get_future());

        int range = max / concurrent_count * (i + 1);
        std::thread t(std::move(task), min, range);
        t.detach();

        min = range + 1;
    }

    std::cout << "threads create finish" << std::endl;

    double sum = 0;
    for (auto& r : results)
    {
        sum += r.get();
    }
    
    result->set_value(sum);
    std::cout << "concurrent_task finish" << std::endl;
}

int main()
{
    auto start_time = std::chrono::steady_clock::now();

    std::promise<double> sum;
    concurrent_task(0, MAX,&sum);

    auto end_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    std::cout << "Concurrent task finish, " << ms << " ms consumed." << std::endl;
    std::cout << "Result: " << sum.get_future().get() << std::endl;

    return 0;
}