# Task A2 - Runtime Binding

## A2.1 Early versus late binding
Early binding resolves a call using the statically known type at compile time. A non-virtual `Bill::total()` call through `Bill*` is bound to the base implementation (or is ill-formed if the base declaration is pure virtual). Late binding selects the final overrider using the dynamic type of the object. Thus, for `Bill* b = new OutPatientBill(...)`, `b->total()` invokes `OutPatientBill::total()` when `total` is virtual.

## A2.2 Conceptual vtables
A compiler's exact ABI layout is implementation-specific. The following is a conceptual representation:

```text
Bill vtable (abstract)
  destructor -> Bill::~Bill
  total       -> pure virtual slot
  type        -> pure virtual slot

OutPatientBill vtable
  destructor -> OutPatientBill::~OutPatientBill
  total       -> OutPatientBill::total
  type        -> OutPatientBill::type

InPatientBill vtable
  destructor -> InPatientBill::~InPatientBill
  total       -> InPatientBill::total
  type        -> InPatientBill::type
```
For `b->total()`, the generated virtual dispatch reads the object's virtual-table pointer, selects the `total` slot, and invokes the derived overrider. `show()` or `print()` need not itself be virtual: its call to a virtual member is dynamically dispatched.

## A2.3 Virtual destructor
Deleting a derived object through `Bill*` requires a virtual base destructor. Otherwise, C++ does not guarantee derived destruction; deleting through a base pointer with a non-virtual destructor is undefined behavior. A virtual destructor makes the derived destructor run first, followed by the base destructor, releasing owned resources correctly.

## Predict-the-output exercise
Prediction, made from the source:
```text
500 ~OPBill ~Bill
```
The non-virtual `show()` calls virtual `total()`, which dispatches to `OPBill::total()`. `delete b` invokes `OPBill::~OPBill` then `Bill::~Bill` because the base destructor is virtual.

If `virtual` is removed from `~Bill`, deleting `OPBill` through `Bill*` is undefined behavior. A common observation may be that only `~Bill` appears, or that the sanitizer diagnoses invalid deletion; neither output is guaranteed. AddressSanitizer is not guaranteed to diagnose every instance of this language-level UB. Do not treat one silent run as proof of safety.

## Alternative: dynamic_cast
With non-virtual type-specific operations, code would attempt a `dynamic_cast<OutPatientBill*>`, then `dynamic_cast<InPatientBill*>`, and so on. In the worst case, up to k casts are tested for k bill types, rather than one virtual dispatch. The object need not carry a vptr solely for `total` dispatch, but RTTI-enabled polymorphic base classes commonly still have runtime type metadata. A vptr typically adds one machine pointer per polymorphic object, although exact layout is ABI-dependent.