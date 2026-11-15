// Q2 - Returning a reference to a local variable (DANGEROUS: undefined behaviour)
// Compile with: g++ -std=c++17 -Wall -Wextra q2_dangling_local.cpp -o q2_dangling_local
// The compiler warns: "reference to local variable 'moderated' returned [-Wreturn-local-addr]"
#include <iostream>

int& moderatedMark(int mark)
{
    int moderated = mark + 2;   // lives in this function's stack frame
    return moderated;           // the frame is destroyed when the function returns
}

int main()
{
    int& r = moderatedMark(91);       // r now refers to memory that no longer exists
    std::cout << "Value read through dangling reference: " << r << '\n';
    // The printed value is NOT guaranteed: it may be 93, garbage, or the program may crash.
}
