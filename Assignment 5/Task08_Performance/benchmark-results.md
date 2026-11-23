# Task B3 - Benchmark Results

## Method
Compiled with `g++ -std=c++17 -O2 -Wall -Wextra`. Executed in this container using `std::chrono::steady_clock`. Same deterministic bill amounts were processed by a base-pointer virtual hierarchy, a tagged structure with `switch`, and `std::variant` with `std::visit`. All checksums agreed. Values are a single run and are sensitive to CPU load, memory allocation and compiler/platform; rerun on the submission machine for repeatable local evidence.

## Bill aggregation timing

| Bills | Virtual (ms) | Switch (ms) | variant/visit (ms) | Checksum |
|---:|---:|---:|---:|---:|
| 1,000 | 0.009334 | 0.000841 | 0.000851 | 69,479 |
| 1,000,000 | 10.0905 | 1.09753 | 1.0492 | 6.95e+07 |
| 10,000,000 | 96.199 | 9.52143 | 9.16374 | 6.95e+08 |

Observed in this run: switch and `variant` were faster than the virtual implementation. The benchmark's polymorphic vector uses one heap allocation per bill (`unique_ptr<Base>`), so the measured difference includes allocation and pointer-indirection/cache-locality effects, not only the virtual call. This is not a pure dispatch microbenchmark. `sizeof(Tagged)=24`, `sizeof(Var)=24`, `sizeof(unique_ptr<Base>)=8` on this build; sizes vary by ABI.

## Validation: return codes versus exceptions

Compiled with `-O2`. All 1,000,000 values were scanned.

| Invalid rate | Return-code path (ms) | Exception path (ms) |
|---:|---:|---:|
| 0% | 0.219771 | 0.476627 |
| 1% | 0.234002 | 0.489536 |
| 50% | 0.215595 | 3.36778 |

For this run, the exception path is much more expensive when errors are frequent, especially at 50%. Exception construction, unwinding and handler execution are not free. At 0% and 1%, exceptions are still somewhat slower in this simple test, but exact results depend strongly on compiler and implementation.

## Interpretation and decision
Both virtual and tagged-switch aggregation are O(n) time. Each bill total is O(1). A virtual hierarchy is valuable when new types are added by separate modules and the processing loop should remain unchanged. A tagged switch can be faster and cache-friendly but each switch must be updated consistently. `std::variant` with `std::visit` provides a closed set of alternatives with compile-time checking and often good locality; adding a variant alternative still requires updating visitors that are not generic.

Return-code validation is preferable when invalid data are routine business outcomes and must be counted/aggregated. Exceptions are suitable for exceptional control-flow and infrastructure failures and keep normal-path code clear.