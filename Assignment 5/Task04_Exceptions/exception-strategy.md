# Task A4 - Exception Strategy

## A4.1 Error inventory and hierarchy
- Negative consultation, test, room, procedure, medicine, package or emergency charge: `InvalidAmountError`.
- Claim exceeds the eligible cover: `ClaimError` (policy may reject or cap according to the explicitly configured rules).
- Unknown day-care/government package code: `UnknownPackageError`.
- Missing or inactive patient record: `PatientRecordError`.
- Allocation failure: standard `std::bad_alloc`.
- Malformed numeric input: standard `std::invalid_argument`; out-of-range values: `std::out_of_range`.

```text
std::exception
└── std::runtime_error
    └── BillingError
        ├── InvalidAmountError
        ├── ClaimError
        ├── UnknownPackageError
        └── PatientRecordError

std::bad_alloc (standard allocation exception)
std::invalid_argument / std::out_of_range (standard input exceptions)
```
All custom messages are carried by `std::runtime_error`.

## A4.2 Throwing contract
Potentially throwing: bill constructors that validate, `total()` if validation is deferred, package lookup, patient lookup, payer calculation, parsing, allocation, file operations and persistence. `print()` may throw if the output stream is configured to throw. `name()`/`type()` can be `noexcept` if they only return fixed string views or literals and do not allocate. Destructors, cleanup, and simple accessors should not throw. Destructors should be `noexcept` because a second exception during stack unwinding causes `std::terminate`; cleanup failures should be logged or handled internally without escaping.

## Alternative: return codes
Use `enum class BillingStatus { ok, invalidAmount, claimRejected, unknownPackage, missingPatient, allocationFailure };` plus an output parameter or a result structure containing status, amount and message. Normal execution still requires O(1) status handling; error paths explicitly branch and propagate the status through every layer. The result structure uses extra storage for status/message and increases repeated checking/boilerplate. Return codes avoid exception-unwinding costs but can be ignored accidentally. Allocation failure is special: even constructing a detailed error string may fail.