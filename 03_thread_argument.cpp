// 03_thread_argument.cpp

#include <iostream>
#include <string>
#include <thread>

void hello(std::string name)
{
    std::cout << "Welcome to " << name << std::endl;
}

int main()
{
    std::thread t(hello, "https://paul.pub");
    t.join();

    return 0;
}