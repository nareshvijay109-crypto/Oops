#include <iostream>
#include <vector>
enum class Zone { Inner, Middle, Outer };
class FareTable {
    const std::vector<long long> base{1000,2000,3000};
public:
    long long get(Zone z, bool peak) const {
        auto b=base.at(static_cast<int>(z));
        return peak ? b+b*20/100 : b;
    }
};
class SmartCard {
    long long balance;
public:
    explicit SmartCard(long long b):balance(b){}
    bool deduct(long long f) {
        if (f<0 || balance-f<5000) return false;
        balance-=f; return true;
    }
    long long getBalance() const { return balance; }
};
int main() {
    FareTable fares; SmartCard card(10000);
    auto f=fares.get(Zone::Outer,true);
    std::cout << "Fare paise=" << f << " accepted=" << std::boolalpha
              << card.deduct(f) << " balance paise=" << card.getBalance() << '\n';
}