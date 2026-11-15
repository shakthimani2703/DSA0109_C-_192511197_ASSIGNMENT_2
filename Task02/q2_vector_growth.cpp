// Q2 - Changed condition: the register is a std::vector that may grow.
// Shows why a reference taken before growth may become invalid (the storage moves).
// We print addresses only - we never use the stale reference, because that is undefined behaviour.
#include <iostream>
#include <vector>

int& highest(std::vector<int>& v)
{
    std::size_t best = 0;
    for (std::size_t i = 1; i < v.size(); ++i)
        if (v[i] > v[best]) best = i;
    return v[best];
}

int main()
{
    std::vector<int> marks = {72, 88, 65, 91};
    std::cout << "size " << marks.size() << ", capacity " << marks.capacity() << '\n';

    int& top = highest(marks);
    const void* oldAddress = &top;
    std::cout << "reference taken to " << top << " at " << oldAddress << '\n';

    for (int m = 50; m < 60; ++m) marks.push_back(m);   // forces reallocation
    std::cout << "after 10 push_back: size " << marks.size() << ", capacity " << marks.capacity() << '\n';
    std::cout << "element 91 now lives at   " << static_cast<const void*>(&marks[3]) << '\n';
    std::cout << (oldAddress != &marks[3] ? "Storage MOVED: 'top' is now dangling.\n"
                                          : "Storage did not move this time.\n");

    // Safe pattern: keep the index and re-access after the vector grows.
    std::size_t idx = 3;
    marks[idx] += 2;
    std::cout << "using the index instead, moderated mark = " << marks[idx] << '\n';
}
