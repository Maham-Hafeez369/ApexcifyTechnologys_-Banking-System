#include <iostream>
using namespace std;
class Customer
{
    // Customer class implementation
private:
    string name;
    int id;
    double balance;

public:
    Customer(string n, int i, double b) : name(n), id(i), balance(b) {}
    void displayInfo()
    {
        cout << "Customer Name: " << name << ", ID: " << id << ", Balance: " << balance << endl;
    }
    void deposit(double amount)
    {
        balance += amount;
    }
    void withdraw(double amount)
    {
        if (amount <= balance)
            balance -= amount;
        else
            cout << "Insufficient funds!" << endl;
    }
    void transfer(Account &toAccount, double amount)
    {
        if (amount <= balance)
        {
            balance -= amount;
            // toAccount.deposit(amount); // Assuming Account class has a deposit method
        }
        else
        {
            cout << "Insufficient funds for transfer!" << endl;
        }
    }
    void viewTransactionHistory()
    {
        // Implementation for viewing transaction history
    }
    void updateDetails(string newName)
    {
        name = newName;
    }
};

class Account
{
    // Account class implementation
    private:
    string accountNumber;   
    double balance;
public:
    Account(string accNum, double bal) : accountNumber(accNum), balance(bal) {}
    void deposit(double amount) {
        balance += amount;
    }       
    void withdraw(double amount) {
        if (amount <= balance)
            balance -= amount;
        else
            cout << "Insufficient funds!" << endl;
    }       
    double getBalance() const {
        return balance;
    }   
    string getAccountNumber() const {
        return accountNumber;
    }   


    
};

class Transaction
{
    // Transaction class implementation
};

int main()
{
    cout << "Hello, Banking System!" << endl;
    return 0;
}
