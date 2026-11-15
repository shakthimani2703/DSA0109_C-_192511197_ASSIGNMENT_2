// Q5 - College Bus Seat Tickets: class design derived from the principle
// "Data that belongs to the whole class should exist only once."
#include <iostream>
#include <string>

class BusTicket {
    // ---- class-wide data: exactly ONE copy, shared by every ticket ----
    static int issued;                 // seats handed out so far
    static int waiting;                // tickets placed on the waiting list
    static const int CAPACITY = 5;     // integral static const: may be initialised in-class

    // ---- per-object data: one copy in EVERY ticket ----
    int seat;                          // 0 means waiting list
    int waitPos;
    std::string name;

public:
    BusTicket() : seat(0), waitPos(0), name("(unassigned)") {}   // needed for arrays without initialisers

    void book(const std::string& n)
    {
        name = n;
        if (issued < CAPACITY) seat = ++issued;
        else                   waitPos = ++waiting;
    }

    void print() const
    {
        std::cout << "  " << name << ": ";
        if (seat) std::cout << "Seat " << seat << '\n';
        else      std::cout << "Waiting list #" << waitPos << '\n';
    }

    // Static member functions: no 'this' pointer, so they can only touch static members.
    static int seatsIssued()   { return issued; }
    static int waitingCount()  { return waiting; }
    static int seatsLeft()     { return CAPACITY - issued; }
    // static int mySeat() { return seat; }   // error: invalid use of member 'seat' in static member function
};

// Definition of the static data members (exactly once, outside the class, in one .cpp file)
int BusTicket::issued = 0;
int BusTicket::waiting = 0;

void bookInFunction(const std::string& who)
{
    BusTicket t;           // local object - destroyed at the end of the function
    t.book(who);
    t.print();             // ...but the static count it changed lives on
}

int main()
{
    std::cout << "Before any ticket: issued = " << BusTicket::seatsIssued() << '\n';

    const char* students[8] = {"Anu", "Bharath", "Charu", "Dinesh", "Elan", "Farah", "Gokul", "Hema"};
    BusTicket tickets[8];                         // array of objects: default constructor runs 8 times
    for (int i = 0; i < 8; ++i) tickets[i].book(students[i]);   // created/booked inside a loop

    std::cout << "Array of 8 tickets, capacity 5:\n";
    for (const BusTicket& t : tickets) t.print();
    std::cout << "Issued = " << BusTicket::seatsIssued()
              << ", waiting = " << BusTicket::waitingCount()
              << ", seats left = " << BusTicket::seatsLeft() << '\n';

    std::cout << "Booking inside a function called twice:\n";
    bookInFunction("Ishaan");
    bookInFunction("Janani");
    std::cout << "Issued = " << BusTicket::seatsIssued()
              << ", waiting = " << BusTicket::waitingCount() << '\n';
    std::cout << "sizeof(BusTicket) = " << sizeof(BusTicket)
              << " bytes (the static counters are NOT inside each object)\n";
}
