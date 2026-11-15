// Q4 - Alternative concept and complexity: shifting array vs circular buffer vs std::deque
// Time to add one transaction when only the last K are kept.
#include <chrono>
#include <cstdio>
#include <deque>
#include <iostream>
#include <vector>

struct Tx { char type; double amount; double bal; };

double shiftArray(int K, int ops)
{
    std::vector<Tx> h(K);
    int count = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < ops; ++i) {
        Tx t{'D', 1.0 * i, 1.0 * i};
        if (count == K) { for (int j = 1; j < K; ++j) h[j - 1] = h[j]; h[K - 1] = t; }
        else h[count++] = t;
    }
    auto t1 = std::chrono::steady_clock::now();
    volatile double sink = h[K - 1].amount; (void)sink;
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
}

double circular(int K, int ops)
{
    std::vector<Tx> h(K);
    int count = 0, start = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < ops; ++i) {
        Tx t{'D', 1.0 * i, 1.0 * i};
        if (count == K) { h[start] = t; start = (start + 1) % K; }
        else h[(start + count++) % K] = t;
    }
    auto t1 = std::chrono::steady_clock::now();
    volatile double sink = h[start].amount; (void)sink;
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
}

double dequeVersion(int K, int ops)
{
    std::deque<Tx> h;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < ops; ++i) {
        h.push_back(Tx{'D', 1.0 * i, 1.0 * i});
        if ((int)h.size() > K) h.pop_front();
    }
    auto t1 = std::chrono::steady_clock::now();
    volatile double sink = h.back().amount; (void)sink;
    return std::chrono::duration<double, std::nano>(t1 - t0).count() / ops;
}

int main()
{
    const int OPS = 1000000;
    std::cout << "ns per transaction (" << OPS << " transactions, g++ -O2)\n";
    std::cout << "     K   shift-array   circular     deque\n";
    for (int K : {10, 50, 500, 5000}) {
        std::printf("%6d %12.2f %10.2f %9.2f\n", K, shiftArray(K, OPS), circular(K, OPS), dequeVersion(K, OPS));
    }
    std::cout << "\nMemory: array/circular = K*" << sizeof(Tx)
              << " bytes fixed; deque = K*" << sizeof(Tx) << " bytes + block map overhead\n";
}
