# Assignment 5 - CO5

**Student:** Naresh Vijay R  
**Register number:** 1972260060  
**Topic:** Reliable Software with Runtime Polymorphism and Exceptions  
**Repository:** https://github.com/nareshvijay109-crypto/Oops

## Compiler
Tested with `g++ (Debian 14.2.0)`. C++17.

## Build and run
Standard:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic Task06_BillingImplementation/todo_solution.cpp -o todo
./todo
```
Full sample billing:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic Task06_BillingImplementation/complete_code.cpp -o billing
./billing
```
Error handling:
```bash
g++ -std=c++17 -Wall -Wextra -pedantic Task07_ErrorTesting/predict_and_complete.cpp -o claims
./claims
```
Benchmarks:
```bash
g++ -std=c++17 -O2 -Wall -Wextra Task08_Performance/benchmark.cpp -o benchmark
./benchmark 1000000
g++ -std=c++17 -O2 -Wall -Wextra Task08_Performance/exception_benchmark.cpp -o exception_benchmark
./exception_benchmark
```
AddressSanitizer:
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -fno-omit-frame-pointer Task06_BillingImplementation/complete_code.cpp -o billing_asan
./billing_asan
```

## Folder map
- `Task01_Design/` - A1 polymorphic hierarchy, UML and complexity
- `Task02_RuntimeBinding/` - A2 binding/vtable explanations and prediction program
- `Task03_Payer/` - A3 payer hierarchy and extension
- `Task04_Exceptions/` - A4 exception hierarchy and policy
- `Task05_Recovery/` - A5 stack unwinding and RAII, prediction program
- `Task06_BillingImplementation/` - B1 TODO solution and full implementation
- `Task07_ErrorTesting/` - B2 validation, throw/catch/rethrow programs
- `Task08_Performance/` - B3 benchmarks and measured results
- `Task09_FinalDecision/` - B4 engineering decision and extension
- `evidence/` - Actual standard-output captures and benchmark records

## Verified outputs
The TODO program produced:
```text
OP Ravi: 1700
IP Meena: 47500
Total 49200
```
Full billing, claim exception, stack unwinding and validation outputs are in `evidence/actual-outputs.txt`.

## Verification notes and limitations
All normal program builds succeeded with the flags above. Benchmarks were run with `-O2` and one observation per dataset; repeat them on your own machine for stronger evidence. AddressSanitizer targets compiled, but this execution environment could not start ASan because it reported that the ASan runtime was not first in the initial library list. Therefore no clean ASan run is claimed. Run the command above locally and add the actual report.

## Submission reminders
The assignment PDF requires progressive commits, input/output screenshots, and a repository named `DSA01-<RegisterNumber>-Assignment5` shared with faculty. This deliverable is placed in the existing `Oops` repository at the student's request. Add faculty as a collaborator before final submission.