#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <iomanip>

class BillingError : public std::runtime_error {
public: using std::runtime_error::runtime_error;
};
class InvalidAmountError : public BillingError {
public: using BillingError::BillingError;
};
class UnknownPackageError : public BillingError {
public: using BillingError::BillingError;
};
class ClaimError : public BillingError {
public: using BillingError::BillingError;
};

class Bill {
protected:
    std::string patient;
    void check(double x) const {
        if (x < 0) throw InvalidAmountError("negative charge");
    }
public:
    explicit Bill(std::string p) : patient(std::move(p)) {}
    virtual ~Bill() noexcept = default;
    virtual double total() const = 0;
    virtual std::string type() const = 0;
    virtual std::string packageCode() const { return {}; }
    virtual void print() const {
        std::cout << type() << ' ' << patient << ": " << total() << '\n';
    }
};
class OutPatientBill : public Bill {
    double consultation, tests;
public:
    OutPatientBill(const std::string& p, double c, double t)
      : Bill(p), consultation(c), tests(t) { check(c); check(t); }
    double total() const override { return consultation + tests; }
    std::string type() const override { return "OP"; }
};
class InPatientBill : public Bill {
    int days; double perDay, procedures, medicines;
public:
    InPatientBill(const std::string& p, int d, double rate, double proc, double med)
      : Bill(p), days(d), perDay(rate), procedures(proc), medicines(med) {
        if (d < 0) throw InvalidAmountError("negative stay duration");
        check(rate); check(proc); check(med);
    }
    double total() const override { return days * perDay + procedures + medicines; }
    std::string type() const override { return "IP"; }
};
class DayCareBill : public Bill {
    std::string code; double packagePrice;
public:
    DayCareBill(const std::string& p, std::string c, double price)
      : Bill(p), code(std::move(c)), packagePrice(price) { check(price); }
    double total() const override { return packagePrice; }
    std::string type() const override { return "DAYCARE"; }
    std::string packageCode() const override { return code; }
};
class EmergencyBill : public Bill {
    double base, hourly; int hours;
public:
    EmergencyBill(const std::string& p, double b, double rate, int h)
      : Bill(p), base(b), hourly(rate), hours(h) {
        check(b); check(rate);
        if (h < 0) throw InvalidAmountError("negative emergency hours");
    }
    double total() const override { return base + hourly * hours; }
    std::string type() const override { return "EMERGENCY"; }
};

// Additional bill type for B4.1; earlier classes are unchanged.
class TelemedicineBill : public Bill {
    double consultation, platformFee;
public:
    TelemedicineBill(const std::string& p, double c, double f)
      : Bill(p), consultation(c), platformFee(f) { check(c); check(f); }
    double total() const override { return consultation + platformFee; }
    std::string type() const override { return "TELEMEDICINE"; }
};

class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual double amountCovered(const Bill&) const = 0;
    virtual std::string name() const = 0;
};
class SelfPay : public Payer {
public:
    double amountCovered(const Bill& b) const override { return b.total(); }
    std::string name() const override { return "SelfPay"; }
};
class Insurance : public Payer {
    double limit, copay;
public:
    Insurance(double l, double p) : limit(l), copay(p) {
        if (l < 0 || p < 0 || p > 100) throw InvalidAmountError("invalid insurance policy");
    }
    double amountCovered(const Bill& b) const override {
        return std::min(b.total() * (1.0 - copay / 100.0), limit);
    }
    std::string name() const override { return "Insurance"; }
};
class GovernmentScheme : public Payer {
    std::map<std::string, double> rates;
public:
    explicit GovernmentScheme(std::map<std::string, double> r) : rates(std::move(r)) {}
    double amountCovered(const Bill& b) const override {
        if (b.packageCode().empty())
            return b.total(); // non-package bills reimbursed at billed total in this sample policy
        auto it = rates.find(b.packageCode());
        if (it == rates.end()) throw UnknownPackageError("unknown package: " + b.packageCode());
        return std::min(b.total(), it->second);
    }
    std::string name() const override { return "GovernmentScheme"; }
};
// Additional payer type for B4.1; no existing class needs modification.
class CorporateTieUp : public Payer {
    double discount;
public:
    explicit CorporateTieUp(double d) : discount(d) {
        if (d < 0 || d > 100) throw InvalidAmountError("invalid corporate discount");
    }
    double amountCovered(const Bill& b) const override {
        return b.total() * (1.0 - discount / 100.0);
    }
    std::string name() const override { return "CorporateTieUp"; }
};

int main() {
    try {
        std::vector<std::unique_ptr<Bill>> bills;
        bills.emplace_back(std::make_unique<OutPatientBill>("Ravi", 500, 1200));
        bills.emplace_back(std::make_unique<InPatientBill>("Meena", 3, 2500, 40000, 0));
        bills.emplace_back(std::make_unique<DayCareBill>("Arun", "DC101", 18000));
        bills.emplace_back(std::make_unique<EmergencyBill>("Leela", 2000, 750, 4));
        bills.emplace_back(std::make_unique<TelemedicineBill>("Kumar", 800, 100));

        double dayTotal = 0;
        for (const auto& b : bills) { b->print(); dayTotal += b->total(); }
        std::cout << "Day total: " << dayTotal << '\n';

        SelfPay self;
        Insurance insurance(50000, 20);
        GovernmentScheme govt({{"DC101", 15000}});
        CorporateTieUp corporate(10);
        std::vector<const Payer*> payers{&self, &insurance, &govt, &corporate};
        for (const auto* p : payers) {
            std::cout << p->name() << " covers OP bill: "
                      << p->amountCovered(*bills.front()) << '\n';
        }

        try {
            OutPatientBill invalid("Test", -10, 50);
            invalid.print();
        } catch (const BillingError& e) {
            std::cout << "validation caught: " << e.what() << '\n';
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "allocation failure: " << e.what() << '\n';
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "billing failure: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "unknown failure\n";
        return 3;
    }
    return 0;
}
