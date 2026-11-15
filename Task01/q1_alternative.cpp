// Q1 - Alternative concept: return the new fee, and measure the cost of
// passing a long long and a large Student object by value / reference / const reference.
#include <chrono>
#include <iostream>

long long applyConcession(long long fee, int percent)   // returns the new fee
{
    return fee - fee * percent / 100;
}

struct BigStudent {
    long long fee;
    char record[4096];          // a large object: marks history, address, photo id ...
};

// noinline keeps the compiler from removing the calls we are measuring
__attribute__((noinline)) long long byValue(BigStudent s)            { return s.fee; }
__attribute__((noinline)) long long byRef(BigStudent& s)             { return s.fee; }
__attribute__((noinline)) long long byConstRef(const BigStudent& s)  { return s.fee; }

template <class F>
double nsPerCall(F f, int n)
{
    auto t0 = std::chrono::steady_clock::now();
    volatile long long sink = 0;
    for (int i = 0; i < n; ++i) sink = sink + f();
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / n;
}

int main()
{
    long long fee = 45000;
    fee = applyConcession(fee, 10);          // merit
    fee = applyConcession(fee, 15);          // sports
    std::cout << "Returned-value version, 45000 after 10% then 15%: " << fee << "\n\n";

    std::cout << "Bytes copied per call\n";
    std::cout << "  long long  by value      : " << sizeof(long long) << '\n';
    std::cout << "  long long  by reference  : 0 (an address of " << sizeof(long long*) << " bytes is passed)\n";
    std::cout << "  BigStudent by value      : " << sizeof(BigStudent) << '\n';
    std::cout << "  BigStudent by (const) ref: 0 (an address of " << sizeof(BigStudent*) << " bytes is passed)\n\n";

    static BigStudent st{};
    st.fee = 45000;
    const int N = 2000000;
    std::cout << "Measured time per call (" << N << " calls)\n";
    std::cout << "  BigStudent by value      : " << nsPerCall([&] { return byValue(st); }, N) << " ns\n";
    std::cout << "  BigStudent by reference  : " << nsPerCall([&] { return byRef(st); }, N) << " ns\n";
    std::cout << "  BigStudent by const ref  : " << nsPerCall([&] { return byConstRef(st); }, N) << " ns\n";
}
