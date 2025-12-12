#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;
double globalBusinessMinBalance = 10000; // Minimum balance for business accounts
double globalMonthlyFee = 10;            // Monthly fee for current accounts
double globalInterestRate = 0.03;        // 3% annual interest rate for savings accounts
int CustomerId = 1;                      // generating IDs
int BankerId = 1;
int AccountId = 1;
int TransactionId = 1;
int nextCustomerId()
{
    return CustomerId++;
}
int nextBankerId()
{
    return BankerId++;
}
int nextAccountId()
{
    return AccountId++;
}
int nextTransactionId()
{
    return TransactionId++;
}
class Person
{
protected:
    string name;
    int id;

public:
    Person(const string &n, const int i) : name(n), id(i) {}
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
    Customer(const string &n, const int i, const string &e, const string &p) : Person(n, i), email(e), pin(p) {}
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
    Employee(const string &n, const int i, const string &r) : Person(n, i), role(r) {}
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
    Banker(const string &n, const int i, const string &r, double limit) : Employee(n, i, r), transferLimit(limit) {}
    bool authorizeSmallTransfer(double amount) // for transfers within limit
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
    Account(int id, Customer *c, double balance = 0.0, bool state = true) : id(id), balance(balance), owner(c), active(state) {};

    virtual ~Account() {} // Virtual destructor for proper cleanup

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
        if (amount <= 0)
        {
            cout << "Invalid deposit amount" << endl;
        }
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

    virtual string getAccountType() = 0;

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
    SavingsAccount(int id, Customer *c, double initialBalance = 0.0, double interestrate = 0.0) : Account(id, c, initialBalance, true), interestRate(interestRate) {
                                                                                                  };

    void setInterestRate(double rate) { interestRate = rate; }
    double getInterestRate() const { return interestRate; }

    void applyMonthlyUpdate() override
    {
        balance += balance * interestRate / 12.0; // monthly interest
    }
    string getAccountType() override
    {
        return "Saving_Account";
    }

    void print() override
    {
        Account::print();
        cout << "Interest Rate: " << interestRate * 100 << "%" << endl;
    }
};
class CurrentAccount : public Account
{
private:
    double monthlyFee;

public:
    CurrentAccount(int id, Customer *c, double initialBalance = 0.0, double monthlyfee = 0) : Account(id, c, initialBalance, true), monthlyFee(monthlyfee) {}

    void setMonthlyFee(double fee) { monthlyFee = fee; }
    double getMonthlyFee() const { return monthlyFee; }

    void applyMonthlyUpdate() override
    {
        balance -= monthlyFee; // Deduct monthly fee
        if (balance < 0)
            balance = 0; // Prevent negative balance
    }
    string getAccountType() override
    {
        return "Current_Account";
    }
    void print()
    {
        Account::print();
        cout << "Monthly Fee: " << monthlyFee << endl;
    }
};
class BusinessAccount : public Account
{
private:
    double minimumBalance;
    double monthlyFee;

public:
    BusinessAccount(int id, Customer *c, double initialBalance = 0.0, double monthlyfee = 0, double minimumbalance = 10000)
        : Account(id, c, initialBalance, true), minimumBalance(minimumbalance), monthlyFee(monthlyfee) {}

    void setMinimumBalance(double minBal) { minimumBalance = minBal; }
    double getMinimumBalance() const { return minimumBalance; }

    void setMonthlyFee(double fee) { monthlyFee = fee; }
    double getMonthlyFee() const { return monthlyFee; }

    void applyMonthlyUpdate() override
    {
        balance -= monthlyFee;
        if (balance < minimumBalance)
            balance = minimumBalance; // Enforce minimum
    }
    string getAccountType() override
    {
        return "Business_Account";
    }

    void print()
    {
        Account::print();
        cout << "Minimum Balance: " << minimumBalance << endl;
        cout << "Monthly Fee: " << monthlyFee << endl;
    }
};
class Transaction
{
private:
    int id;
    int fromAccountId;
    int toAccountId;
    double amount;
    string type;      // "DEPOSIT", "WITHDRAW", "TRANSFER"
    string timestamp; // stored as readable string

public:
    Transaction(int id, int fromAcc, int toAcc, double amount, string type)
    {
        this->id = id;
        this->fromAccountId = fromAcc;
        this->toAccountId = toAcc;
        this->amount = amount;
        this->type = type;

        // Generate timestamp
        time_t now;
        time(&now);                                         // get current time in variable now
        timestamp = ctime(&now);                            // convert to string
        if (!timestamp.empty() && timestamp.back() == '\n') // Remove newline from ctime()
            timestamp.pop_back();
    }

    int getId()
    {
        return id;
    }

    int getFromAccountId()
    {
        return fromAccountId;
    }

    int getToAccountId()
    {
        return toAccountId;
    }

    double getAmount()
    {
        return amount;
    }

    string getType()
    {
        return type;
    }

    string getTimestamp()
    {
        return timestamp;
    }

    void print()
    {
        cout << "=== Transaction ===" << endl;
        cout << "ID: " << id << endl;
        cout << "Type: " << type << endl;
        cout << "From Account: " << fromAccountId << endl;
        cout << "To Account: " << toAccountId << endl;
        cout << "Amount: " << amount << endl;
        cout << "Timestamp: " << timestamp << endl;
    }
};
Customer *createCustomer()
{
    string name, email, pin;
    cout << "Enter Your name: ";
    cin >> name;
    cout << "Enter Your email: ";
    cin >> email;
    cout << "Set your pin: ";
    cin >> pin;
    Customer c(name, nextCustomerId(), email, pin);
    return &c;
}
Account *createSavingsAccount(Customer *c)
{

    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    SavingsAccount sa = SavingsAccount(nextAccountId(), c, initialBalance, globalInterestRate);
    return &sa;
}
Account *createCurrentAccount(Customer *c)
{
    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    CurrentAccount ca = CurrentAccount(nextAccountId(), c, initialBalance, globalMonthlyFee);
    return &ca;
}
Account *createBusinessAccount(Customer *c)
{
    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    BusinessAccount ba = BusinessAccount(nextAccountId(), c, initialBalance, globalMonthlyFee, globalBusinessMinBalance);

    return &ba;
}

int main()
{

    return 0;
}