// Q6 - Train Journey Planner: friend + inline functions with correctness tests.
#include <iomanip>
#include <iostream>

class Time {
    int h, m;                                       // private: valid range enforced below
public:
    Time(int hh, int mm) : h(hh), m(mm)
    {
        if (h < 0 || h > 23 || m < 0 || m > 59) { h = 0; m = 0; }   // invariant protected
    }
    // Defined inside the class -> implicitly inline; friend -> may read private h and m.
    friend inline int minutesBetween(const Time& a, const Time& b)
    {
        int d = (a.h * 60 + a.m) - (b.h * 60 + b.m);
        return d < 0 ? -d : d;
    }
    friend inline bool earlier(const Time& a, const Time& b)
    {
        return a.h * 60 + a.m < b.h * 60 + b.m;
    }
    void print() const { std::cout << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m << std::setfill(' '); }
};

int failures = 0;

void check(const Time& a, const Time& b, int expMin, bool expEarlier)
{
    int got = minutesBetween(a, b);
    bool e = earlier(a, b);
    a.print(); std::cout << " vs "; b.print();
    std::cout << " -> minutes " << std::setw(4) << got << " (expected " << std::setw(4) << expMin << "), earlier "
              << std::boolalpha << e << " (expected " << expEarlier << ")  "
              << ((got == expMin && e == expEarlier) ? "PASS" : "FAIL") << '\n';
    if (got != expMin || e != expEarlier) ++failures;
}

int main()
{
    check(Time(9, 45),  Time(13, 10), 205, true);    // either side of noon
    check(Time(13, 10), Time(9, 45),  205, false);   // reversed order: difference is still positive
    check(Time(11, 50), Time(12, 5),   15, true);    // just before / just after noon
    check(Time(12, 0),  Time(12, 0),    0, false);   // same time twice: not earlier
    check(Time(7, 30),  Time(7, 30),    0, false);   // same time twice (morning)
    check(Time(0, 0),   Time(23, 59), 1439, true);   // whole day
    check(Time(12, 30), Time(12, 31),   1, true);    // one minute apart
    std::cout << (failures == 0 ? "All tests passed\n" : "Some tests FAILED\n");
}
