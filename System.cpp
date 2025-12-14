#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <fstream>
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
   int customerID;
public:
    Account(int id, int c, double balance = 0.0, bool state = true) : id(id), balance(balance), customerID(c), active(state) {};

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

    void setOwner(int c)
    {
        customerID = c;
    }

    int getOwner()
    {
        return customerID;
    }

    virtual void print()
    {
        cout << "Account ID: " << id << endl;
        cout << "Balance: " << balance << endl;
        cout << "Status: " << (active ? "Active" : "Locked") << endl;
        ifstream in("customer_" + to_string(customerID) + ".txt");
        if (in.is_open())
        {
            string var;
            in>>var;
            cout<<"Name of Owner:"<<var<<endl;
        }
    }
};
class SavingsAccount : public Account
{
private:
    double interestRate; // annual interest rate
public:
    SavingsAccount(int id, int c, double initialBalance = 0.0, double interestrate = 0.0) : Account(id, c, initialBalance, true), interestRate(interestRate) {
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
    CurrentAccount(int id, int c, double initialBalance = 0.0, double monthlyfee = 0) : Account(id, c, initialBalance, true), monthlyFee(monthlyfee) {}

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
    BusinessAccount(int id, int c, double initialBalance = 0.0, double monthlyfee = 0, double minimumbalance = 10000)
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
    Transaction(int id, int fromAcc, double amount, string type, int toAcc = 0)
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
    int id = nextCustomerId();
    Customer *c = new Customer(name, id, email, pin);
    ofstream outFile("customer_" + to_string(id) + ".txt");
    if (outFile.is_open())
    {
        outFile << name << endl;
        outFile << email << endl;
        outFile << id << endl;
        outFile << pin << endl;
        cout << "Customer data saved to file." << endl;
    }
    else
    {
        cout << "Error saving customer data to file." << endl;
    }
    outFile.close();
    return c;
}
Account *createSavingsAccount(int  cstmrID)
{

    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    SavingsAccount *sa = new SavingsAccount(nextAccountId(), cstmrID, initialBalance, globalInterestRate);
    ofstream outFile("savings_account_" + to_string(sa->getId()) + ".txt");
    if (outFile.is_open())
    {
        outFile << sa->getId() << endl;
        outFile << initialBalance << endl;
        outFile << globalInterestRate << endl;
        outFile << cstmrID << endl;
        cout << "Savings_account data saved to file." << endl;
    }
    else
    {
        cout << "Error saving savings_account data to file." << endl;
    }
    outFile.close();
    return sa;
}
Account *createCurrentAccount(int cstmrID)
{
    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    CurrentAccount *ca = new CurrentAccount(nextAccountId(), cstmrID, initialBalance, globalMonthlyFee);
    ofstream outFile("current_account_" + to_string(ca->getId()) + ".txt");
    if (outFile.is_open())
    {
        outFile << ca->getId() << endl;
        outFile << initialBalance << endl;
        outFile << globalMonthlyFee << endl;
        outFile << cstmrID << endl;
        cout << "Current_account data saved to file." << endl;
    }
    else
    {
        cout << "Error saving current_account data to file." << endl;
    }
    outFile.close();
    return ca;
}
Account *createBusinessAccount(int cstmrID)
{
    double initialBalance;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    BusinessAccount *ba = new BusinessAccount(nextAccountId(), cstmrID, initialBalance, globalMonthlyFee, globalBusinessMinBalance);
    ofstream outFile("business_account_" + to_string(ba->getId()) + ".txt");
    if (outFile.is_open())
    {
        outFile << ba->getId() << endl;
        outFile << initialBalance << endl;
        outFile << globalMonthlyFee << endl;
        outFile << globalBusinessMinBalance << endl;
        outFile << cstmrID << endl;
        cout << "Business_account data saved to file." << endl;
    }
    else
    {
        cout << "Error saving business_account data to file." << endl;
    }
    outFile.close();
    return ba;
}
void displayMainMenu()
{
    cout << "=== Banking System Main Menu ===" << endl;
    cout << "1. Create Customer" << endl;
    cout << "2. Create Account" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Withdraw" << endl;
    cout << "5. Transfer" << endl;
    cout << "6. View Account Details" << endl;
    cout << "7. View Transaction History" << endl;
    cout << "8. Exit" << endl;
    cout << "Select an option: ";
}
void handleAccountCreation(int accType, int cstmrID)
{
    Account *newAccount = nullptr;
    switch (accType)
    {
    case 1:
        newAccount = createSavingsAccount(cstmrID);
        break;
    case 2:
        newAccount = createCurrentAccount(cstmrID);
        break;
    case 3:
        newAccount = createBusinessAccount(cstmrID);
        break;
    default:
        cout << "Invalid account type selected." << endl;
        return;
    }
    if (newAccount != nullptr)
    {
        ofstream file("customer_"+to_string(cstmrID)+".txt", ios::app);
        file << newAccount->getId() << endl;
        file.close();
        cout << newAccount->getAccountType() << " created with ID: " << newAccount->getId() << "for User :" << cstmrID << endl;
    }
}
void handleDeposit()
{
    cout << "Enter Customer ID: " << endl;
    int custmrID;
    cin >> custmrID;
    cout<<"Enter Account Type (1-Savings,2-Current,3-Business):"<<endl;
    int accType;
    cin>>accType;
    cout << "Enter Account ID: " << endl;
    int accID;
    cin >> accID;
    cout << "Enter amount to deposit: " << endl;
    double amount;
    cin >> amount;
    double balance;
    ifstream in;   // file stream object
    switch (accType)
    {
    case 1:
        in.open("savings_account"+to_string(accID)+".txt");
        if(in.is_open())
        {
            in>>accID;
            in>>balance;
        }
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;

    default:
        break;
    }

   



}
void handleChoice(int choice)
{
    switch (choice)
    {
    case 1:
    {
        Customer *c = createCustomer();
        cout << "Customer created with ID: " << c->getId() << endl;
        break;
    }
    case 2:
    {
        cout << "Select Account Type:" << endl;
        cout << "1. Savings Account" << endl;
        cout << "2. Current Account" << endl;
        cout << "3. Business Account" << endl;
        int accType;
        cin >> accType;
        cout << "Give a Customer ID to link this account to:" << endl;
        int custmrID;
        cin >> custmrID;
        handleAccountCreation(accType, custmrID);
        break;
    }
    case 3:
    {
        cout << "Deposit:" << endl;
        break;
    }
    case 4:
    {
        cout << "Withdraw selected." << endl;
        break;
    }
    case 5:
    {
        cout << "Transfer selected." << endl;
        break;
    }
    case 6:
    {
        cout << "View Account Details selected." << endl;
        break;
    }
    case 7:
    {
        cout << "View Transaction History selected." << endl;
        break;
    }
    case 8:
    {
        cout << "Exiting system. Goodbye!" << endl;
        break;
    }
    default:
    {
        cout << "Invalid choice. Please try again." << endl;
        break;
    }
    }
}
void runSystem()
{
    displayMainMenu();
    int choice;
    cin >> choice;
    handleChoice(choice);
}
int main()
{

    return 0;
}