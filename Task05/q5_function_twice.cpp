// Q5 - Follow-up to "Complete the Code": tickets created inside a function that is called twice.
#include <iostream>
#include <string>

class BusTicket {
    static int issued;
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        if (issued < CAPACITY) seat = ++issued;
    }
    static int count() { return issued; }
    void print() const
    {
        std::cout << "  " << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting")) << '\n';
    }
};

int BusTicket::issued = 0;

void morningTrip(int call)
{
    std::cout << "Call " << call << " of morningTrip():\n";
    BusTicket a("Asha"), b("Bala");          // local objects, destroyed when the function returns
    a.print();
    b.print();
    std::cout << "  Issued so far: " << BusTicket::count() << '\n';
}

int main()
{
    morningTrip(1);
    morningTrip(2);      // the objects from call 1 are gone, but 'issued' kept its value
    std::cout << "Final issued: " << BusTicket::count() << '\n';
}
