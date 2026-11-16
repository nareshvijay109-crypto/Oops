#include <iostream>
#include <string>
enum class Zone { Inner, Middle, Outer };
constexpr long long BASE[] = {1000,2000,3000};
constexpr long long MIN_BALANCE = 5000;
bool validZone(Zone z) {
    int index=static_cast<int>(z);
    return index >= 0 && index < 3;
}
long long fare(Zone z, int hour) {
    if (!validZone(z) || hour < 0 || hour > 23) return -1;
    long long b = BASE[static_cast<int>(z)];
    bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);
    return peak ? b + b*20/100 : b;
}
bool canTravel(long long bal,long long f) {
    return f >= 0 && bal >= f && bal-f >= MIN_BALANCE;
}
bool recharge(long long& balance, long long amount) {
    if (amount < 0) return false;
    balance += amount;
    return true;
}
int main() {
    struct Test { const char* name; Zone zone; int hour; long long balance; long long expected; bool allowed; };
    Test tests[] = {
      {"Inner offpeak",Zone::Inner,11,10000,1000,true},
      {"Middle peak 8AM",Zone::Middle,8,10000,2400,true},
      {"Outer peak 9AM",Zone::Outer,9,10000,3600,true},
      {"10AM boundary",Zone::Outer,10,10000,3000,true},
      {"5PM boundary",Zone::Inner,17,10000,1200,true},
      {"8PM boundary",Zone::Inner,20,10000,1000,true},
      {"exact Rs50 left",Zone::Outer,10,8000,3000,true},
      {"low balance",Zone::Outer,10,7999,3000,false}
    };
    int passed=0;
    for (const auto& t:tests) {
      long long actual=fare(t.zone,t.hour);
      bool allowed=canTravel(t.balance,actual);
      bool ok=(actual==t.expected && allowed==t.allowed);
      std::cout << t.name << ": fare_paise=" << actual
                << " allowed=" << std::boolalpha << allowed
                << " [" << (ok?"PASS":"FAIL") << "]\n";
      passed += ok;
    }
    std::cout << "Passed " << passed << "/" << (sizeof(tests)/sizeof(tests[0])) << '\n';
    long long fiveJourneyTotal = fare(Zone::Inner,11) + fare(Zone::Middle,8)
                               + fare(Zone::Outer,9) + fare(Zone::Middle,11)
                               + fare(Zone::Outer,11);
    std::cout << "Five-journey total Rs " << fiveJourneyTotal/100 << '\n';
    std::cout << "Unknown zone rejected=" << (fare(static_cast<Zone>(99),12)<0) << '\n';
    long long wallet=10000;
    std::cout << "Negative recharge rejected=" << (!recharge(wallet,-500)) << '\n';
}