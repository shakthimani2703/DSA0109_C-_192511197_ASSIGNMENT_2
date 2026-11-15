// Q2 - Examination Marks Register: return by reference, const reference and copy
#include <iostream>

const int N = 6;

// Returns a reference to the highest mark: the caller can change the array element.
int& highest(int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

// Returns a const reference: no copy, but the caller cannot change the element.
const int& highestReadOnly(const int marks[], int n)
{
    int best = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[best]) best = i;
    return marks[best];
}

// Returns a copy: changing the result never touches the array.
int highestCopy(const int marks[], int n)
{
    return highestReadOnly(marks, n);
}

void print(const int marks[], int n)
{
    for (int i = 0; i < n; ++i) std::cout << marks[i] << ' ';
    std::cout << '\n';
}

int main()
{
    int marks[N] = {72, 88, 65, 91, 79, 84};
    std::cout << "Original register     : ";
    print(marks, N);

    highest(marks, N) += 2;                 // moderation applied through the reference
    std::cout << "After highest() += 2  : ";
    print(marks, N);

    highest(marks, N) = 95;                 // the call used on the left of =
    std::cout << "After highest() = 95  : ";
    print(marks, N);

    const int& top = highestReadOnly(marks, N);
    // top += 2;   // error: assignment of read-only reference 'top'
    std::cout << "Const reference reads : " << top << '\n';

    int copy = highestCopy(marks, N);
    copy += 2;                              // changes only the copy
    std::cout << "Copy changed to       : " << copy << '\n';
    std::cout << "Register unchanged    : ";
    print(marks, N);

    marks[3] = 60;                          // the reference follows the element, not the value
    std::cout << "top after marks[3]=60 : " << top << "  (it still names marks[3])\n";
}
