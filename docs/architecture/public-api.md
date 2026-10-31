# Public API

The naming scheme itself is described in
[`../system/public-api.md`](../system/public-api.md).

## Naming

- Every exported function MUST start with `bas_` and MUST be mangled as
  `bas_<name>[_<type>...]`, with lower-case parameter-type tokens in
  declaration order.
  *Source: [ADR 0003](../adr/0003-bas-symbol-naming-scheme.md).*
- A support function whose parameters are not BASIC types MUST have the `bas_`
  prefix and MUST NOT have type tokens.
  *Source: [ADR 0003](../adr/0003-bas-symbol-naming-scheme.md).*
- The garbage collector MUST keep its `jcc_gc_*` names.
  *Source: [ADR 0003](../adr/0003-bas-symbol-naming-scheme.md). JCC's
  `RuntimeGcCodeGenerator` emits those names for both BASIC and COL.*
- Two exported functions MUST NOT differ only in return type, because the
  scheme does not encode the return type.
  *Source: [ADR 0003](../adr/0003-bas-symbol-naming-scheme.md).*
- Error messages MUST name the BASIC function, for example `chr$`, and MUST
  NOT name the exported symbol.
  *Source: the BASIC programmer reads these messages and never sees the
  `bas_` symbols.*

## Compatibility with JCC

JCC-emitted LLVM IR calls libjccbas functions by symbol name, and JCC does not
compile against the headers.

- Renaming or removing an exported symbol MUST be treated as an ABI break and
  MUST raise the major version.
  *Source: JCC links by symbol name. The 3.0.0 rename raised the major
  version for this reason.*
