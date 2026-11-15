// Q2 - Alternative concept: return the highest mark as an index or as a pointer.
#include <iostream>
#include <vector>

int highestIndex(const int marks[], int n)          // O(n) time, O(1) space
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return best;
}

int* highestPointer(int marks[], int n)             // O(n) time, O(1) space
{
    if (n <= 0) return nullptr;                     // a pointer can say "no answer"
    return &marks[highestIndex(marks, n)];
}

int main()
{
    int marks[] = {72, 88, 65, 91, 79, 84};
    const int n = 6;

    int i = highestIndex(marks, n);
    marks[i] += 2;                                  // caller indexes the array itself
    std::cout << "index version  : marks[" << i << "] = " << marks[i] << '\n';

    int* p = highestPointer(marks, n);
    if (p) *p += 2;                                 // caller must check for nullptr
    std::cout << "pointer version: *p = " << *p << '\n';

    // With a growing vector, only the index survives a reallocation.
    std::vector<int> v(marks, marks + n);
    std::size_t vi = highestIndex(v.data(), n);
    v.reserve(1000);                                // storage moves; pointers/references to it dangle
    v[vi] += 1;                                     // the index is still valid
    std::cout << "after growth, index still valid: v[" << vi << "] = " << v[vi] << '\n';
}
