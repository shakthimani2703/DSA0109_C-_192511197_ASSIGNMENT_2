// Q4 - Savings Account Mini-Statement: a class with an array inside it.
// Two strategies for the 11th transaction are shown: SHIFT the array, or use a CIRCULAR index.
#include <iomanip>
#include <iostream>
#include <string>

struct Transaction {
    char type;          // 'D' deposit, 'W' withdrawal
    double amount;
    double balanceAfter;
};

class SavingsAccount {
    static const int MAX = 10;          // last 10 transactions only
    std::string accountNo;
    double balance;
    Transaction history[MAX];           // array within a class - private
    int count;                          // how many valid entries (0..MAX)
    bool useCircular;                   // which strategy this object uses
    int start;                          // circular buffer: index of the oldest entry

    void record(char type, double amount);   // defined outside the class
    void recordShift(const Transaction& t);
    void recordCircular(const Transaction& t);

public:
    // Defined INSIDE the class (implicitly inline)
    SavingsAccount(const std::string& no, double opening, bool circular = false)
        : accountNo(no), balance(opening), count(0), useCircular(circular), start(0) {}

    double getBalance() const { return balance; }

    // Declared here, defined OUTSIDE the class with the scope resolution operator ::
    void deposit(double amount);
    bool withdraw(double amount);
    void miniStatement() const;
};

void SavingsAccount::deposit(double amount)
{
    if (amount <= 0) return;
    balance += amount;
    record('D', amount);
}

bool SavingsAccount::withdraw(double amount)
{
    if (amount <= 0 || amount > balance) {
        std::cout << "  Withdrawal of " << amount << " refused (insufficient balance)\n";
        return false;
    }
    balance -= amount;
    record('W', amount);
    return true;
}

void SavingsAccount::record(char type, double amount)
{
    Transaction t{type, amount, balance};
    if (useCircular) recordCircular(t);
    else             recordShift(t);
}

// Shift strategy: when full, move every element one place left - O(MAX) per transaction.
void SavingsAccount::recordShift(const Transaction& t)
{
    if (count == MAX) {
        for (int i = 1; i < MAX; ++i) history[i - 1] = history[i];
        history[MAX - 1] = t;
    } else {
        history[count++] = t;
    }
}

// Circular strategy: overwrite the oldest entry and advance 'start' - O(1) per transaction.
void SavingsAccount::recordCircular(const Transaction& t)
{
    if (count == MAX) {
        history[start] = t;
        start = (start + 1) % MAX;
    } else {
        history[(start + count) % MAX] = t;
        ++count;
    }
}

void SavingsAccount::miniStatement() const
{
    std::cout << "Mini-statement for A/c " << accountNo
              << (useCircular ? " (circular index)" : " (shifting array)") << '\n';
    for (int i = 0; i < count; ++i) {
        const Transaction& t = useCircular ? history[(start + i) % MAX] : history[i];
        std::cout << "  " << std::setw(2) << i + 1 << ". " << (t.type == 'D' ? "Deposit " : "Withdraw")
                  << std::setw(10) << t.amount << "   Bal " << std::setw(10) << t.balanceAfter << '\n';
    }
    std::cout << "  Current balance: " << balance << "\n\n";
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    SavingsAccount shiftAcc("SB1001", 5000.0);
    SavingsAccount ringAcc("SB1002", 5000.0, true);

    // 12 transactions so the 11th and 12th push out the two oldest.
    double amounts[12] = {1000, 200, 1500, 300, 2500, 400, 700, 100, 900, 50, 1200, 600};
    for (int i = 0; i < 12; ++i) {
        if (i % 2 == 0) { shiftAcc.deposit(amounts[i]);  ringAcc.deposit(amounts[i]); }
        else            { shiftAcc.withdraw(amounts[i]); ringAcc.withdraw(amounts[i]); }
    }
    shiftAcc.withdraw(100000);           // refused, not recorded

    shiftAcc.miniStatement();
    ringAcc.miniStatement();
}
