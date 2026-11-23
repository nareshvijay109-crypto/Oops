#include <chrono>
#include <iostream>
#include <stdexcept>
#include <vector>
volatile long long checksum=0;
template<class F> void measure(const char* label,F f){
    auto a=std::chrono::steady_clock::now(); f();
    auto b=std::chrono::steady_clock::now();
    std::cout<<label<<": "<<std::chrono::duration<double,std::milli>(b-a).count()<<" ms\n";
}
int main(){
    constexpr int N=1000000;
    for(int invalidPercent: {0,1,50}) {
        std::vector<int> values(N,1);
        for(int i=0;i<N*invalidPercent/100;i+=100) values[i]=-1;
        measure(("return-code "+std::to_string(invalidPercent)+"%").c_str(),[&]{
            long long s=0; for(int v:values){if(v<0)continue;s+=v;}checksum=s;
        });
        measure(("exception "+std::to_string(invalidPercent)+"%").c_str(),[&]{
            long long s=0; for(int v:values){try{if(v<0)throw std::invalid_argument("bad");s+=v;}
            catch(const std::exception&) {}} checksum=s;
        });
    }
}
