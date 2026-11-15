// Q6 - Alternative concepts: member functions (a.minutesTo(b), a.isEarlier(b)) and public getters.
#include <iostream>

class TimeM {                                   // design 2: member functions
    int h, m;
public:
    TimeM(int hh, int mm) : h(hh), m(mm) {}
    int minutesTo(const TimeM& o) const { int d = (h * 60 + m) - (o.h * 60 + o.m); return d < 0 ? -d : d; }
    bool isEarlier(const TimeM& o) const { return h * 60 + m < o.h * 60 + o.m; }
};

class TimeG {                                   // design 3: public getters
    int h, m;
public:
    TimeG(int hh, int mm) : h(hh), m(mm) {}
    int hour() const { return h; }
    int minute() const { return m; }
};
int minutesBetween(const TimeG& a, const TimeG& b)     // ordinary non-friend function
{
    int d = (a.hour() * 60 + a.minute()) - (b.hour() * 60 + b.minute());
    return d < 0 ? -d : d;
}
bool earlier(const TimeG& a, const TimeG& b)
{
    return a.hour() * 60 + a.minute() < b.hour() * 60 + b.minute();
}

int main()
{
    TimeM dm(9, 45), am(13, 10);
    std::cout << std::boolalpha;
    std::cout << "member : " << dm.minutesTo(am) << ' ' << am.isEarlier(dm) << '\n';
    TimeG dg(9, 45), ag(13, 10);
    std::cout << "getters: " << minutesBetween(dg, ag) << ' ' << earlier(ag, dg) << '\n';
}
