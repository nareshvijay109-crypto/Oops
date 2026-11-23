#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    const long long farePaise=3000;
    for (long long n : {20000LL,500000LL,3000000LL}) {
        long long total=farePaise*n;
        std::cout << n << " journeys: Rs " << total/100
                  << " (" << total << " paise)\n";
    }
    std::cout << "INT_MAX=" << INT_MAX << '\n';
    double sum=0.0;
    for (int i=0;i<10;++i) sum += 0.1;
    std::cout << std::setprecision(17) << "0.1 added 10 times=" << sum << '\n';
}