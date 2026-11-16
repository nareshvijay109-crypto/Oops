#include <iostream>
bool proceduralCanTravel(long long balance, long long fare) {
    return fare >= 0 && balance - fare >= 5000;
}
class SmartCard {
    long long balancePaise;
    static constexpr long long minimum = 5000;
public:
    explicit SmartCard(long long balance) : balancePaise(balance) {}
    bool canTravel(long long fare) const {
        return fare >= 0 && balancePaise - fare >= minimum;
    }
    bool deduct(long long fare) {
        if (!canTravel(fare)) return false;
        balancePaise -= fare; return true;
    }
    long long balance() const { return balancePaise; }
};
int main() {
    long long proceduralBalance = 10000, f = 3600;
    if (proceduralCanTravel(proceduralBalance,f)) proceduralBalance -= f;
    SmartCard card(10000); card.deduct(f);
    std::cout << "Procedural balance=" << proceduralBalance
              << "\nOOP balance=" << card.balance() << '\n';
}