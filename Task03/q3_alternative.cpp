// Q3 - Alternative concepts: (1) one function taking a Parcel structure with optional
// fields, (2) a function template that handles any weight type.
#include <iomanip>
#include <iostream>
#include <type_traits>

struct ParcelRequest {
    double kg = 0.0;
    bool express = false;      // optional fields have default member initialisers
    bool insured = false;
    bool fragile = false;      // a NEW option: added here only, no new overloads needed
};

double charge(const ParcelRequest& r)
{
    double c = r.kg * 40.0;
    if (c < 50.0) c = 50.0;
    if (r.express) c += 60.0;
    if (r.insured) c += 25.0;
    if (r.fragile) c += 30.0;
    return c;
}

// Template: integral types are treated as grams, floating types as kilograms.
template <class W>
double chargeT(W weight, bool express = false, bool insured = false)
{
    static_assert(std::is_arithmetic<W>::value, "weight must be a number");
    ParcelRequest r;
    if constexpr (std::is_integral<W>::value) r.kg = weight / 1000.0;
    else                                      r.kg = static_cast<double>(weight);
    r.express = express;
    r.insured = insured;
    return charge(r);
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    ParcelRequest a;            a.kg = 2.5;
    ParcelRequest b;            b.kg = 1.2; b.express = true;
    ParcelRequest c;            c.kg = 3.2; c.express = true; c.insured = true; c.fragile = true;
    std::cout << "struct  2.5 kg                       : " << charge(a) << '\n';
    std::cout << "struct  1.2 kg express               : " << charge(b) << '\n';
    std::cout << "struct  3.2 kg express+insured+fragile: " << charge(c) << '\n';

    std::cout << "template chargeT(500)   [W=int]      : " << chargeT(500) << '\n';
    std::cout << "template chargeT(2.5)   [W=double]   : " << chargeT(2.5) << '\n';
    std::cout << "template chargeT(2.0f)  [W=float]    : " << chargeT(2.0f) << '\n';
    std::cout << "template chargeT(750L,true) [W=long] : " << chargeT(750L, true) << '\n';
    std::cout << "sizeof(ParcelRequest) = " << sizeof(ParcelRequest) << " bytes (copied only if passed by value)\n";
}
