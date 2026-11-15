// Q3 - The developer's original design. THIS FILE DOES NOT COMPILE (on purpose).
// g++ -std=c++17 -Wall -Wextra q3_ambiguous.cpp  ->  error: call of overloaded 'charge(double)' is ambiguous
#include <iostream>

double charge(double weight)                                   { return 40 * weight; }
double charge(double weight, bool express)                     { return 40 * weight + (express ? 60 : 0); }
double charge(double weight, bool express = false, bool insured = false)
{
    return 40 * weight + (express ? 60 : 0) + (insured ? 25 : 0);
}

int main()
{
    std::cout << charge(2.5) << '\n';          // matches #1 and #3 (with both defaults)
    std::cout << charge(2.5, true) << '\n';    // matches #2 and #3 (with one default)
}
