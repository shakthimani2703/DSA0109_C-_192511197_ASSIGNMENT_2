// Q6 - Growth investigation: 10,000,000 calls with and without inlining,
// for friend functions, member functions and public getters.
// Build twice:  g++ -std=c++17 -O0 q6_inline_benchmark.cpp -o bench_O0
//               g++ -std=c++17 -O2 q6_inline_benchmark.cpp -o bench_O2
#include <chrono>
#include <iostream>

#define NOINLINE __attribute__((noinline))

class Time {
    int h, m;
public:
    Time(int hh = 0, int mm = 0) : h(hh), m(mm) {}
    // friend versions
    friend inline int minutesInline(const Time& a, const Time& b);
    friend int minutesNoInline(const Time& a, const Time& b);
    // member-function version
    int minutesTo(const Time& b) const { int d = (h * 60 + m) - (b.h * 60 + b.m); return d < 0 ? -d : d; }
    // getter version
    int hour() const { return h; }
    int minute() const { return m; }
};

inline int minutesInline(const Time& a, const Time& b)
{
    int d = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return d < 0 ? -d : d;
}

NOINLINE int minutesNoInline(const Time& a, const Time& b)
{
    int d = (a.h * 60 + a.m) - (b.h * 60 + b.m);
    return d < 0 ? -d : d;
}

inline int minutesGetters(const Time& a, const Time& b)
{
    int d = (a.hour() * 60 + a.minute()) - (b.hour() * 60 + b.minute());
    return d < 0 ? -d : d;
}

const int N = 10000000;
const int POOL = 1024;
Time pool[POOL];

template <class F>
double run(const char* label, F f)
{
    long long sum = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) sum += f(pool[i & (POOL - 1)], pool[(i * 7) & (POOL - 1)]);
    auto t1 = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "  " << label << ": " << ms << " ms  (checksum " << sum << ")\n";
    return ms;
}

int main()
{
    for (int i = 0; i < POOL; ++i) pool[i] = Time((i * 37) % 24, (i * 53) % 60);
    std::cout << "10,000,000 calls each\n";
    run("friend, inline    ", [](const Time& a, const Time& b) { return minutesInline(a, b); });
    run("friend, noinline  ", [](const Time& a, const Time& b) { return minutesNoInline(a, b); });
    run("member function   ", [](const Time& a, const Time& b) { return a.minutesTo(b); });
    run("public getters    ", [](const Time& a, const Time& b) { return minutesGetters(a, b); });
}
