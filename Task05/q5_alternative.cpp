// Q5 - Alternative concepts: (A) a global counter, (B) a TicketOffice object passed to every ticket.
#include <iostream>
#include <string>

// ---------- (A) global variable ----------
int g_issued = 0;                         // anyone, anywhere can change this
const int G_CAPACITY = 3;

struct GlobalTicket {
    int seat = 0;
    explicit GlobalTicket() { if (g_issued < G_CAPACITY) seat = ++g_issued; }
};

// ---------- (B) TicketOffice object ----------
class TicketOffice {
    std::string route;
    int capacity;
    int issued = 0;
public:
    TicketOffice(const std::string& r, int cap) : route(r), capacity(cap) {}
    int nextSeat() { return issued < capacity ? ++issued : 0; }
    int count() const { return issued; }
    const std::string& name() const { return route; }
};

class OfficeTicket {
    std::string name;
    int seat;
public:
    OfficeTicket(TicketOffice& office, const std::string& n) : name(n), seat(office.nextSeat()) {}
    std::string text() const { return name + (seat ? ": Seat " + std::to_string(seat) : ": Waiting"); }
};

int main()
{
    GlobalTicket t1, t2;
    g_issued = 0;                          // accidental reset from unrelated code - nothing prevents it
    GlobalTicket t3;
    std::cout << "(A) global: t1 seat " << t1.seat << ", t2 seat " << t2.seat
              << ", t3 seat " << t3.seat << "  <- duplicate seat 1 after a stray reset\n";

    TicketOffice busA("Route 12 (Tambaram)", 2), busB("Route 7 (Avadi)", 3);
    OfficeTicket a1(busA, "Asha"), a2(busA, "Bala"), a3(busA, "Chitra");
    OfficeTicket b1(busB, "Dev"),  b2(busB, "Elan");
    std::cout << "(B) " << busA.name() << ": " << a1.text() << " | " << a2.text() << " | " << a3.text()
              << "  issued " << busA.count() << '\n';
    std::cout << "(B) " << busB.name() << ": " << b1.text() << " | " << b2.text()
              << "  issued " << busB.count() << '\n';
}
