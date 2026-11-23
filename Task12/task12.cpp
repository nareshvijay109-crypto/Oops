#include <array>
#include <iostream>
enum class Zone { Inner, Middle, Outer, Airport };
constexpr std::array<long long,4> BASE_PAISE{1000,2000,3000,6000};
class MonthlyPass {
    std::array<bool,4> covered{};
public:
    MonthlyPass(Zone a, Zone b) {
        covered[static_cast<int>(a)]=true;
        covered[static_cast<int>(b)]=true;
    }
    bool covers(Zone z) const { return covered[static_cast<int>(z)]; }
};
long long revisedFare(Zone z, bool peak, bool weekend, const MonthlyPass* pass) {
    if (pass && pass->covers(z)) return 0;
    long long base=BASE_PAISE[static_cast<int>(z)];
    if (weekend) return base - base*10/100;
    return peak ? base + base*20/100 : base;
}
int main() {
    MonthlyPass pass(Zone::Inner,Zone::Middle);
    std::cout << "Airport weekday peak Rs " << revisedFare(Zone::Airport,true,false,nullptr)/100 << '\n';
    std::cout << "Outer weekend peak Rs " << revisedFare(Zone::Outer,true,true,nullptr)/100 << '\n';
    std::cout << "Inner covered by pass Rs " << revisedFare(Zone::Inner,true,false,&pass)/100 << '\n';
}