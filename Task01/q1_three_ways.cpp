// Q1 - Hostel Fee Concession Updater
// The concession function written three ways: by value, by reference, by pointer.
#include <iostream>
#include <string>

// 1) Call by value: 'fee' is a new local variable (a copy) in this function's stack frame.
void concessionByValue(long long fee, int percent)
{
    fee -= fee * percent / 100;
    std::cout << "  inside byValue    : " << fee << '\n';   // looks correct here...
}

// 2) Call by reference: 'fee' is another name (alias) for the caller's variable.
void concessionByReference(long long& fee, int percent)
{
    fee -= fee * percent / 100;
    std::cout << "  inside byReference: " << fee << '\n';
}

// 3) Call by pointer: 'fee' holds the address of the caller's variable.
void concessionByPointer(long long* fee, int percent)
{
    if (fee == nullptr) return;          // a pointer can be null, so we must check
    *fee -= *fee * percent / 100;
    std::cout << "  inside byPointer  : " << *fee << '\n';
}

// Design decision: return the new fee so concessions can be chained and the
// original value is never lost by accident.
long long withConcession(long long fee, int percent)
{
    return fee - fee * percent / 100;
}

struct Student {
    std::string name;
    std::string regNo;
    long long fee;
};

// const reference: read-only access, no copy of the whole Student object.
void printStudent(const Student& s)
{
    std::cout << s.name << " (" << s.regNo << ") fee = " << s.fee << '\n';
}

int main()
{
    const int MERIT = 10, SPORTS = 15, SINGLE_PARENT = 25;

    long long fee = 60000;
    std::cout << "Starting fee: " << fee << "\n\n";

    std::cout << "Call by value (merit 10%):\n";
    concessionByValue(fee, MERIT);
    std::cout << "  stored fee after call: " << fee << "\n\n";

    std::cout << "Call by reference (merit 10%):\n";
    concessionByReference(fee, MERIT);
    std::cout << "  stored fee after call: " << fee << "\n\n";

    std::cout << "Call by pointer (sports 15%):\n";
    concessionByPointer(&fee, SPORTS);
    std::cout << "  stored fee after call: " << fee << "\n\n";

    // Several concessions for one student, using the value-returning design.
    Student s{"Kavin", "192511001", 60000};
    std::cout << "Chained concessions with the returning version:\n";
    long long finalFee = withConcession(withConcession(withConcession(s.fee, MERIT), SPORTS), SINGLE_PARENT);
    std::cout << "  original fee kept : " << s.fee << '\n';
    std::cout << "  fee after all three: " << finalFee << '\n';
    s.fee = finalFee;                  // caller decides when to commit the change
    printStudent(s);
    return 0;
}
