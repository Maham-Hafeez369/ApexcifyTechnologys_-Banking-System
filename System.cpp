#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class Person
{
protected:
    string name;
    int id;

public:
    string getName()
    {
        return name;
    }

    int getId()
    {
        return id;
    }

    void setName(string newName)
    {
        name = newName;
    }

    void setId(int newId)
    {
        id = newId;
    }

    void print()
    {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }
};
class Customer : public Person
{
private:
    string email;
    string pin;
    vector<int> accountIds;

public:
    string getEmail()
    {
        return email;
    }

    void setEmail(string e)
    {
        email = e;
    }

    void setPin(string p)
    {
        pin = p;
    }

    bool verifyPin(string p)
    {
        return p == pin;
    }

    void addAccountId(int id)
    {
        accountIds.push_back(id);
    }

    vector<int> getAccountIds()
    {
        return accountIds;
    }

    void print()
    {
        Person::print();
        cout << "Email: " << email << endl;
        cout << "Accounts: ";
        for (int id : accountIds)
            cout << id << " ";
        cout << endl;
    }
};
class Employee : public Person
{
protected:
    string role;
    string username;
    string password;

public:
    string getRole()
    {
        return role;
    }

    void setRole(string r)
    {
        role = r;
    }

    void setLoginCredentials(string u, string p)
    {
        username = u;
        password = p;
    }

    bool authenticate(string u, string p)
    {
        return (u == username && p == password);
    }

    void print()
    {
        Person::print();
        cout << "Role: " << role << endl;
    }
};
class Banker : public Employee
{
private:
    double transferLimit;

public:
    bool authorizeSmallTransfer(double amount)
    {
        return amount <= transferLimit;
    }

    double getTransferLimit()
    {
        return transferLimit;
    }

    void setTransferLimit(double limit)
    {
        transferLimit = limit;
    }

    void print()
    {
        Employee::print();
        cout << "Transfer Limit: " << transferLimit << endl;
    }
};
class Account
{
protected:
    int id;
    double balance;
    bool active;
    Customer *owner;

public:
    Account()
    {
        id = 0;
        balance = 0.0;
        active = true;
        owner = nullptr;
    }

    virtual ~Account() {}

    int getId()
    {
        return id;
    }

    double getBalance()
    {
        return balance;
    }

    bool isActive()
    {
        return active;
    }

    void lock()
    {
        active = false;
    }

    void unlock()
    {
        active = true;
    }

    void deposit(double amount)
    {
        if (!active)
        {
            cout << "Inactive account";
            return;
        }
        if (amount > 0)
            balance += amount;
    }

    bool withdraw(double amount)
    {
        if (!active || amount <= 0 || amount > balance)
            return false;
        balance -= amount;
        return true;
    }

    // PURE VIRTUAL → makes class abstract
    virtual void applyMonthlyUpdate() = 0;

    //  virtual string getAccountType() = 0;

    void setOwner(Customer *c)
    {
        owner = c;
    }

    Customer *getOwner()
    {
        return owner;
    }

    virtual void print()
    {
        cout << "Account ID: " << id << endl;
        cout << "Balance: " << balance << endl;
        cout << "Status: " << (active ? "Active" : "Locked") << endl;
        if (owner != nullptr)
        {
            cout << "Owner: " << owner->getName() << endl;
        }
    }
};
class SavingsAccount : public Account
{
private:
    double interestRate; // annual interest rate
public:
    SavingsAccount() {};
    SavingsAccount(int id, double initialBalance = 0.0) : id(id), balance(initialBalance), interestRate(0.0), owner(nullptr), active(true) {}
    // this->id = id;
    // this->balance = initialBalance;
    // interestRate = 0.0;
    // owner = nullptr;
    // active = true;
    //}
    void setInterestRate(double rate) { interestRate = rate; }
    double getInterestRate() const { return interestRate; }

    void applyMonthlyUpdate() override
    {
        balance += balance * interestRate / 12.0; // monthly interest
    }

    void print() override
    {
        cout << "Savings Account ID: " << id << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Interest Rate: " << interestRate * 100 << "%" << endl;
        cout << "Status: " << (active ? "Active" : "Locked") << endl;
    }

    int getId() const { return id; }
    double getBalance() const { return balance; }

    void setOwner(Customer *c) { owner = c; }
    Customer *getOwner() const { return owner; }
    void lock() { active = false; }
    void unlock() { active = true; }
};
class CurrentAccount : public Account
{
private:
    int id;
    double balance;
    double monthlyFee;
    Customer *owner;
    bool active;

public:
    CurrentAccount() {}
    CurrentAccount(int id, double initialBalance = 0.0)
        : id(id), balance(initialBalance), monthlyFee(0.0), owner(nullptr), active(true) {}

    void setMonthlyFee(double fee) { monthlyFee = fee; }
    double getMonthlyFee() const { return monthlyFee; }

    void deposit(double amount)
    {
        if (!active)
            throw runtime_error("Account locked");
        balance += amount;
    }

    void withdraw(double amount)
    {
        if (!active)
            throw runtime_error("Account locked");
        if (amount > balance)
            throw runtime_error("Insufficient funds");
        balance -= amount;
    }

    void applyMonthlyUpdate() override
    {
        balance -= monthlyFee; // Deduct monthly fee
        if (balance < 0)
            balance = 0; // Prevent negative balance
    }

    void print() const
    {
        cout << "Current Account ID: " << id << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Monthly Fee: " << monthlyFee << endl;
        cout << "Status: " << (active ? "Active" : "Locked") << endl;
    }

    int getId() const { return id; }
    double getBalance() const { return balance; }

    void setOwner(Customer *c) { owner = c; }
    Customer *getOwner() const { return owner; }
    void lock() { active = false; }
    void unlock() { active = true; }
};
class BusinessAccount : public Account
{
private:
    int id;
    double balance;
    double minimumBalance;
    double monthlyFee;
    Customer *owner;
    bool active;

public:
    BusinessAccount() {};
    BusinessAccount(int id, double initialBalance = 0.0)
        : id(id), balance(initialBalance), minimumBalance(0.0), monthlyFee(0.0), owner(nullptr), active(true) {}

    void setMinimumBalance(double minBal) { minimumBalance = minBal; }
    double getMinimumBalance() const { return minimumBalance; }

    void setMonthlyFee(double fee) { monthlyFee = fee; }
    double getMonthlyFee() const { return monthlyFee; }

    void deposit(double amount)
    {
        if (!active)
            throw runtime_error("Account locked");
        balance += amount;
    }

    void withdraw(double amount)
    {
        if (!active)
            throw runtime_error("Account locked");
        if ((balance - amount) < minimumBalance)
            throw runtime_error("Cannot go below minimum balance");
        balance -= amount;
    }

    void applyMonthlyUpdate() override
    {
        balance -= monthlyFee;
        if (balance < minimumBalance)
            balance = minimumBalance; // Enforce minimum
    }

    void print() const
    {
        cout << "Business Account ID: " << id << endl;
        cout << "Balance: " << fixed << setprecision(2) << balance << endl;
        cout << "Minimum Balance: " << minimumBalance << endl;
        cout << "Monthly Fee: " << monthlyFee << endl;
        cout << "Status: " << (active ? "Active" : "Locked") << endl;
    }

    int getId() const { return id; }
    double getBalance() const { return balance; }

    void setOwner(Customer *c) { owner = c; }
    Customer *getOwner() const { return owner; }
    void lock() { active = false; }
    void unlock() { active = true; }
};

int main()
{

    return 0;
}