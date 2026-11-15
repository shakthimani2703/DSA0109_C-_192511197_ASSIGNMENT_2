// Q7 - Diagnose, Redesign and Validate
// Selected program: Q4 SavingsAccount (fixed array, shifted on every transaction once full).
//
// ORIGINAL  : capacity fixed at compile time (template parameter here so we can test
//             several sizes); when full, every transaction shifts K-1 entries -> O(K).
// REDESIGNED: TransactionLog class owns a ring buffer whose capacity is chosen at run
//             time (constructor argument); adding a transaction is O(1) for any K.
#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

struct Transaction {
    char type;
    double amount;
    double balanceAfter;
    bool operator==(const Transaction& o) const
    {
        return type == o.type && amount == o.amount && balanceAfter == o.balanceAfter;
    }
};

// ======================= ORIGINAL DESIGN (from Q4) =======================
template <int MAX>
class OriginalAccount {
    double balance;
    Transaction history[MAX];
    int count = 0;

    void record(char t, double amt)
    {
        Transaction tx{t, amt, balance};
        if (count == MAX) {
            for (int i = 1; i < MAX; ++i) history[i - 1] = history[i];   // O(MAX) shift
            history[MAX - 1] = tx;
        } else {
            history[count++] = tx;
        }
    }

public:
    explicit OriginalAccount(double opening) : balance(opening) {}
    void deposit(double a) { if (a > 0) { balance += a; record('D', a); } }
    bool withdraw(double a)
    {
        if (a <= 0 || a > balance) return false;
        balance -= a; record('W', a); return true;
    }
    std::vector<Transaction> statement() const { return std::vector<Transaction>(history, history + count); }
};

// ======================= REDESIGNED =======================
class TransactionLog {                       // our own ring buffer (not a library container)
    std::unique_ptr<Transaction[]> buf;      // owns the storage; freed automatically
    int cap;
    int start = 0;                           // index of the oldest entry
    int count = 0;
public:
    explicit TransactionLog(int capacity) : buf(new Transaction[capacity]), cap(capacity) {}
    void add(const Transaction& t)           // O(1) regardless of capacity
    {
        if (count < cap) buf[(start + count++) % cap] = t;
        else { buf[start] = t; start = (start + 1) % cap; }
    }
    int size() const { return count; }
    int capacity() const { return cap; }
    const Transaction& at(int i) const { return buf[(start + i) % cap]; }   // 0 = oldest
};

class RedesignedAccount {
    double balance;
    TransactionLog log;
public:
    RedesignedAccount(double opening, int keepLast) : balance(opening), log(keepLast) {}
    void deposit(double a) { if (a > 0) { balance += a; log.add({'D', a, balance}); } }
    bool withdraw(double a)
    {
        if (a <= 0 || a > balance) return false;
        balance -= a; log.add({'W', a, balance}); return true;
    }
    std::vector<Transaction> statement() const
    {
        std::vector<Transaction> v;
        for (int i = 0; i < log.size(); ++i) v.push_back(log.at(i));
        return v;
    }
};

// ======================= SAME TESTS FOR BOTH =======================
template <int K>
bool sameStatements(int ops, unsigned seed)
{
    OriginalAccount<K> o(10000);
    RedesignedAccount r(10000, K);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> amt(1, 3000), kind(0, 1);
    for (int i = 0; i < ops; ++i) {
        double a = amt(rng);
        if (kind(rng)) { o.deposit(a); r.deposit(a); }
        else           { o.withdraw(a); r.withdraw(a); }
    }
    return o.statement() == r.statement();
}

template <int K>
void timeBoth(int ops)
{
    static OriginalAccount<K> o(1e12);       // static: large K does not overflow the stack
    RedesignedAccount r(1e12, K);

    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < ops; ++i) o.deposit(1.0 + (i & 7));
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < ops; ++i) r.deposit(1.0 + (i & 7));
    auto t2 = std::chrono::steady_clock::now();

    double origNs = std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
    double newNs  = std::chrono::duration<double, std::nano>(t2 - t1).count() / ops;
    std::printf("%6d %14.2f %14.2f %10.1fx %10zu %10zu\n", K, origNs, newNs, origNs / newNs,
                sizeof(OriginalAccount<K>), sizeof(RedesignedAccount) + K * sizeof(Transaction));
}

int main()
{
    std::cout << "Correctness: identical mini-statements for 100,000 random transactions\n";
    std::cout << "  K=10   : " << (sameStatements<10>(100000, 1) ? "MATCH" : "DIFFER") << '\n';
    std::cout << "  K=50   : " << (sameStatements<50>(100000, 2) ? "MATCH" : "DIFFER") << '\n';
    std::cout << "  K=500  : " << (sameStatements<500>(100000, 3) ? "MATCH" : "DIFFER") << '\n';

    const int OPS = 1000000;
    std::cout << "\nTime per transaction (" << OPS << " deposits, g++ -O2) and memory\n";
    std::cout << "     K    original(ns) redesigned(ns)    speed-up  orig bytes   new bytes\n";
    timeBoth<10>(OPS);
    timeBoth<50>(OPS);
    timeBoth<500>(OPS);
    timeBoth<5000>(OPS);

    std::cout << "\nRun-time capacity: one compiled program serves both banks' policies\n";
    RedesignedAccount atm(5000, 10), branch(5000, 50);
    for (int i = 1; i <= 60; ++i) { atm.deposit(i); branch.deposit(i); }
    std::cout << "  ATM account keeps " << atm.statement().size()
              << ", branch account keeps " << branch.statement().size() << " transactions\n";
}
