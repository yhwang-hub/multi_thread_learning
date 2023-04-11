// 15_parallel_algorithm.cpp

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
#include <random>
#include <execution>

void generateRandomData(std::vector<double>& collection, int size)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(1.0, 100.0);
    for (int i = 0; i < size; i++)
    {
        collection.push_back(dist(mt));
    }
}

int main()
{
    std::vector<double> collection;
    generateRandomData(collection, 10e6);

    std::vector<double> copy1(collection);
    std::vector<double> copy2(collection);
    std::vector<double> copy3(collection);

    auto time1 = std::chrono::steady_clock::now();
    std::sort(std::execution::seq, copy1.begin(), copy1.end());
    auto time2 = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    std::cout << "Sequenced sort consuming" << duration << "ms." << std::endl;

    auto time3 = std::chrono::steady_clock::now();
    std::sort(std::excution::par, copy2.begin(), copy2.end());
    auto time4 = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(time4 - time3).count();
    std::cout << "Parallel sort consuming" << duration << "ms." << std::endl;

    auto time5 = std::chrono::steady_clock::now();
    std::sort(std::execution::par_unseq, copy2.begin(), copy2.end());
    auto time6 = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(time6 - time5).count();
    std::cout << "Parallel unsequenced sort consuming" << duration << "ms." << std::endl;

    return 0;
}