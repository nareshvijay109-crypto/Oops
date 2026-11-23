# Task B4 - Final Engineering Decision

## Extension demonstration
`complete_code.cpp` adds `TelemedicineBill` and `CorporateTieUp` as new derived classes. The existing bill and payer classes and processing loop are not modified. This is the intended extension property of interface-based polymorphism.

## Resource-safety approach
The implementation stores bills in `std::unique_ptr` inside `std::vector`, uses a virtual `noexcept` base destructor, and catches failures at the application boundary. The local validation failure is caught as a `BillingError`. Run the sanitizer command in the README on a compatible compiler; a clean run is evidence for the exercised paths only, not a proof for every possible input.

## Production decision
For 15 hospitals, multiple independently evolving bill types and payers, and 200,000 bills/day, the polymorphic design is suitable if correctness and maintainability dominate tiny dispatch costs. O(n) daily aggregation is straightforward. Exception handling is appropriate for exceptional invalid inputs and system failures, but routine business rejection can be represented with an explicit result type if invalid claims are frequent. Money should be represented in integer minor units or a decimal type, rules must be versioned and audited, and persistence must be transactional and idempotent. The sample is an educational in-memory model, not a production-ready hospital billing system.

## Alternative: configuration-driven plug-ins
A rules engine could load bill/payer formulas and package rates from validated configuration. Each bill is evaluated by interpreting a rule, which may cost more than a virtual function call; configuration storage grows with rule count and complexity. It allows authorized rate changes without recompiling, but introduces parsing, schema, versioning, validation and deployment risks. C++ classes are safer for complex workflows and invariants; configuration is preferable for simple rates and package tables. A hybrid is sensible: stable bill behaviours in code, hospital-specific rates and package tables in signed/versioned configuration.