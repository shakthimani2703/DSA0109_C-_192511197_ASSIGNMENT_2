// Q3 - Courier Charge Calculator, redesigned: overloading by TYPE of weight,
// default arguments for the optional services, and no ambiguous call.
#include <iomanip>
#include <iostream>
#include <string>

struct Parcel {
    std::string trackingId;
    double kg;
    bool express;
    bool insured;
};

const double RATE_PER_KG = 40.0, MINIMUM = 50.0, EXPRESS = 60.0, INSURANCE = 25.0;

double baseCharge(double kg, bool express, bool insured)
{
    double c = kg * RATE_PER_KG;
    if (c < MINIMUM) c = MINIMUM;
    if (express) c += EXPRESS;
    if (insured) c += INSURANCE;
    return c;
}

// (A) weight in grams as an int
double charge(int grams, bool express = false, bool insured = false)
{
    std::cout << "[charge(int grams)]     ";
    return baseCharge(grams / 1000.0, express, insured);
}

// (B) weight in kilograms as a double
double charge(double kg, bool express = false, bool insured = false)
{
    std::cout << "[charge(double kg)]     ";
    return baseCharge(kg, express, insured);
}

// (C) a whole Parcel object (passed by const reference: no copy)
double charge(const Parcel& p)
{
    std::cout << "[charge(const Parcel&)] ";
    return baseCharge(p.kg, p.express, p.insured);
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);
    Parcel p{"TN0425", 3.2, true, true};

    std::cout << "1. charge(500)        -> " << charge(500) << '\n';
    std::cout << "2. charge(2.5)        -> " << charge(2.5) << '\n';
    std::cout << "3. charge(2)          -> " << charge(2) << '\n';
    std::cout << "4. charge(2.0f)       -> " << charge(2.0f) << '\n';
    std::cout << "5. charge(1.2, true)  -> " << charge(1.2, true) << '\n';
    std::cout << "6. charge(p)          -> " << charge(p) << '\n';
    std::cout << "7. charge(750,true,true) -> " << charge(750, true, true) << '\n';
}
