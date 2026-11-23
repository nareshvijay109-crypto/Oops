#include <iomanip>
#include <iostream>
#include <limits>
void discardBadLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
int main() {
    int hour{};
    std::cout << "Enter hour (0-23): ";
    if (!(std::cin >> hour)) {
        std::cout << "Invalid numeric input\n";
        discardBadLine();
        return 1;
    }
    if (hour < 0 || hour > 23) {
        std::cout << "Hour out of range\n"; return 1;
    }
    long long paise = 1234;
    std::cout << "Fare Rs " << paise/100 << '.' << std::setw(2)
              << std::setfill('0') << paise%100 << '\n';
}