#include <iostream>
enum class Zone { Inner, Middle, Outer };
constexpr long long INNER_FARE_PAISE = 1000;
constexpr long long MIDDLE_FARE_PAISE = 2000;
constexpr long long OUTER_FARE_PAISE = 3000;
constexpr int PEAK_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;
int main() {
    Zone z = Zone::Outer;
    long long fare = (z == Zone::Outer) ? OUTER_FARE_PAISE : INNER_FARE_PAISE;
    std::cout << "Outer fare paise=" << fare
              << ", peak percent=" << PEAK_PERCENT
              << ", minimum balance paise=" << MIN_BALANCE_PAISE << '\n';
}