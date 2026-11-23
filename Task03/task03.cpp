#include <climits>
#include <iostream>
int main() {
    std::cout << "sizeof(bool)=" << sizeof(bool) << '\n'
              << "sizeof(char)=" << sizeof(char) << '\n'
              << "sizeof(int)=" << sizeof(int) << '\n'
              << "sizeof(long long)=" << sizeof(long long) << '\n'
              << "sizeof(float)=" << sizeof(float) << '\n'
              << "sizeof(double)=" << sizeof(double) << '\n'
              << "INT_MAX=" << INT_MAX << '\n'
              << "LLONG_MAX=" << LLONG_MAX << '\n';
    long long totalPaise = 3000LL * 3000000LL;
    std::cout << "3 million Rs30 fares in paise=" << totalPaise << '\n';
}