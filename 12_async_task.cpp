// 12_async_task.cpp

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
static double sum = 0;

void worker(int min, int max)
{
    for (int i = min; i <= max; i++)
    {
        /* code */
        sum += std::sqrt(i);
    }
}

class Worker
{
private:
    int mMin;
    int mMax;
    double mResult;

public:
    Worker(int min, int max): mMin(min), mMax(max) {};
    double work()
    {
        mResult = 0;
        for (int i = mMin; i <= mMax; i++)
        {
            mResult += std::sqrt(i);
        }
        return mResult;
    }

    double getResult()
    {
        return mResult;
    }
};

int main()
{
    sum = 0;
    auto f1 = std::async(worker, 0, MAX);
    std::cout << "Async task triggerd" << std::endl;
    f1.wait();
    std::cout << "Async task finish, result: " << sum << std::endl;

    double result = 0;
    std::cout << "Async task with lambda triggerd, thread: "
            << std::this_thread::get_id()
            << std::endl;
    auto f2 = std::async(std::launch::async, [&result]() {
        std::cout << "lambda task in thread: "
                << std::this_thread::get_id()
                << std::endl;
        for (int i = 0; i <= MAX; i++)
        {
            result += std::sqrt(i);
        }
    });
    f2.wait();
    std::cout << "Async task with lambda finish, result: "
            << result << std::endl;

    Worker w(0, MAX);
    std::cout << "Task in class triggered" << std::endl;
    auto f3 = std::async(&Worker::work, &w);
    f3.wait();
    std::cout << "Task in class finish, result: " << w.getResult() << std::endl;

    return 0;
}