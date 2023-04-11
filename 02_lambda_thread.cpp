// 02_lambda_thread.cpp

#include <iostream>
#include <thread>

int main()
{
    std::thread t(
        [] {std::cout << "Hello world from lambda thread." << std::endl;}
    );
    t.join();

    return 0;
}