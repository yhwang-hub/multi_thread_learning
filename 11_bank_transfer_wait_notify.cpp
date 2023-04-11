// 11_bank_transfer_wait_notify.cpp

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

class Account
{
private:
    std::string mName;
    double mMoney;
    std::mutex mMoneyLock;
    std::condition_variable mConditionVar;

public:
    Account(std::string name, double money): mName(name), mMoney(money) {};

    void changeMoney(double amount)
    {
        std::unique_lock<std::mutex> lock(mMoneyLock);
        mConditionVar.wait(lock, [this, amount] {return mMoney + amount > 0;});

        mMoney += amount;

        mConditionVar.notify_all();
    }

    std::string getName()
    {
        return mName;
    }

    std::mutex* getLock()
    {
        return &mMoneyLock;
    }

    double getMoney()
    {
        return mMoney;
    }
};

class Bank
{
private:
    std::set<Account*> mAccounts;

public:
    void addAccount(Account* account)
    {
        mAccounts.insert(account);
    }

    bool transferMoney(Account* accountA, Account* accountB, double amount)
    {

        accountA->changeMoney(-amount);
        accountB->changeMoney(amount);
    }

    double totalMoney() const
    {
        double sum = 0;
        for (auto a : mAccounts)
        {
            sum += a->getMoney();
        }
        
        return sum;
    }
};


std::mutex sCountLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB)
{
    while (true)
    {
        double randomMoney  = ((double)rand() / RAND_MAX) * 100;
        {
            std::lock_guard<std::mutex> guard(sCountLock);
            std::cout << "Try to Transfer " << randomMoney
                    << " from " << accountA->getName() << "(" << accountA->getMoney()
                    << ") to " << accountB->getName() << "(" << accountB->getMoney()
                    << "), Bank totalMoney: " << bank->totalMoney() << std::endl;
        }
        bank->transferMoney(accountA, accountB, randomMoney);
    }
}

int main()
{
    Account a("Paul", 100);
    Account b("Moira", 100);

    Bank aBank;
    aBank.addAccount(&a);
    aBank.addAccount(&b);

    std::thread t1(randomTransfer, &aBank, &a, &b);
    std::thread t2(randomTransfer, &aBank, &b, &a);

    t1.join();
    t2.join();

    return 0;
}