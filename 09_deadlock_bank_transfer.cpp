// 09_deadlock_bank_transfer.cpp

#include <iostream>
#include <iomanip>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <set>

class Account
{
private:
    std::string mName;
    double mMoney;
    std::mutex mMoneyLock;

public:
    Account(std::string name, double money): mName(name), mMoney(money) {};

    void changeMoney(double amount)
    {
        mMoney += amount;
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
        std::lock_guard<std::mutex> guardA(*accountA->getLock());
        std::lock_guard<std::mutex> guardB(*accountB->getLock());

        if (amount > accountA->getMoney())
        {
            return false;
        }
        
        accountA->changeMoney(-amount);
        accountB->changeMoney(amount);
        return true;
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

void randomTransfer(Bank* bank, Account* accountA, Account* accountB)
{
    while (true)
    {
        double randomMoney  = ((double)rand() / RAND_MAX) * 100;
        if (bank->transferMoney(accountA, accountB, randomMoney))
        {
            std::cout << "Transfer " << randomMoney << " from " << accountA->getName()
                << " to " << accountB->getName()
                << ", Bank totalMoney: " << bank->totalMoney() << std::endl;
        }
        else
        {
            std::cout << "Transfer failed, "
                << accountA->getName() << " has only $" << accountA->getMoney() << ", but "
                << randomMoney << " required" << std::endl;
        }
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