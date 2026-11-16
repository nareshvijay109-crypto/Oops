#include <iostream>
#include <string>
struct Passenger { std::string name; bool student; bool senior; };
struct Journey { int originZone; int farthestZone; int hour; bool valid; };
class SmartCard {
    long long balancePaise;
public:
    explicit SmartCard(long long b) : balancePaise(b) {}
    bool canTravel(long long fare) const { return balancePaise - fare >= 5000; }
    bool deduct(long long fare) { if (!canTravel(fare)) return false; balancePaise -= fare; return true; }
    long long balance() const { return balancePaise; }
};
int main() {
    Passenger p{"Passenger A", true, false};
    Journey j{1, 3, 9, true};
    SmartCard c{10000};
    std::cout << p.name << " zone " << j.farthestZone
              << " travel=" << std::boolalpha << c.deduct(1200)
              << " balance_paise=" << c.balance() << '\n';
}