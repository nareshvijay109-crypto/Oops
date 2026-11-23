# Task A5 - Recovery, Rethrowing and RAII

## A5.1 Catch locations
1. Validation functions throw precise domain exceptions and do not print user-facing messages.
2. The billing service catches a domain exception only when it can add useful audit context; it logs a safe identifier and rethrows with `throw;` to preserve the dynamic exception type.
3. The application boundary catches known exceptions, reports a clear message to the operator/patient, marks the transaction failed, and leaves the original bill record intact for correction/retry.
4. A final `catch (...)` is reserved for the process boundary to prevent an exception escaping `main`; it reports an unknown failure and returns a failure status. It must not conceal errors silently.

## A5.2 Stack unwinding and RAII
When an exception leaves a scope, automatic objects in that scope are destroyed in reverse construction order. RAII types such as `std::string`, `std::vector`, `std::unique_ptr`, file streams and lock guards release their resources. This is safer than manually issuing `delete` inside every catch branch because a newly added error path does not need a matching cleanup statement.

## Predict-the-output exercise
Prediction:
```text
~f ~main caught 42
```
`f()` constructs `g`, then throws integer 42. During stack unwinding, `g` is destroyed first and prints `~f`. Control leaves the try block, so the `Guard m` in `main` is destroyed and prints `~main`. The `catch(int)` handler prints `caught 42`. `not printed` is unreachable because the throw transfers control immediately to a matching handler.

## Alternative: manual delete in catch blocks
Manual cleanup can have O(1) cost per resource, like RAII. However, every return, throw, and catch branch must be audited. A new error path creates another opportunity to omit deletion. `unique_ptr` and containers centralize ownership and keep the cleanup paths independent of the number of exceptions.