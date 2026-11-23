#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <variant>
#include <vector>

struct Base { virtual ~Base()=default; virtual double total() const=0; };
struct OP final: Base { double a,b; OP(double x,double y):a(x),b(y){} double total()const override{return a+b;} };
struct IP final: Base { double a,b; IP(double x,double y):a(x),b(y){} double total()const override{return a+b;} };
struct Tagged { int type; double a,b; };
struct VOP { double a,b; }; struct VIP { double a,b; };
using Var = std::variant<VOP,VIP>;
double visit(const Var& v) { return std::visit([](const auto& x){return x.a+x.b;},v); }

template<class F> double timed(const char* label, F f) {
    auto t0=std::chrono::steady_clock::now(); volatile double sink=f();
    auto t1=std::chrono::steady_clock::now();
    auto ms=std::chrono::duration<double,std::milli>(t1-t0).count();
    std::cout<<label<<": "<<ms<<" ms (checksum "<<sink<<")\n"; return ms;
}
int main(int argc,char**argv) {
    std::size_t n=argc>1?std::stoull(argv[1]):1000000;
    std::vector<std::unique_ptr<Base>> poly; poly.reserve(n);
    std::vector<Tagged> tagged; tagged.reserve(n);
    std::vector<Var> vars; vars.reserve(n);
    for(std::size_t i=0;i<n;++i) {
        double a=(i%100)+10,b=(i%17)+2;
        if(i%2){poly.emplace_back(std::make_unique<OP>(a,b));tagged.push_back({0,a,b});vars.emplace_back(VOP{a,b});}
        else {poly.emplace_back(std::make_unique<IP>(a,b));tagged.push_back({1,a,b});vars.emplace_back(VIP{a,b});}
    }
    timed("virtual", [&]{double s=0;for(auto& p:poly)s+=p->total();return s;});
    timed("switch", [&]{double s=0;for(auto& x:tagged)s+=(x.type==0?x.a+x.b:x.a+x.b);return s;});
    timed("variant_visit", [&]{double s=0;for(auto& x:vars)s+=visit(x);return s;});
    std::cout<<"sizeof(Tagged)="<<sizeof(Tagged)<<" sizeof(Var)="<<sizeof(Var)
             <<" sizeof(unique_ptr<Base>)="<<sizeof(std::unique_ptr<Base>)<<"\n";
}
