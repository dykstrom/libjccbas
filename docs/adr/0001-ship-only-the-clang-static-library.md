# 0001. Ship only the Clang static library

*2026-10-12*

## Context

On Windows, libjccbas shipped two libraries. GCC/MinGW built `libjccbas.dll`, linked against `msvcrt.dll` for executables from the JCC FASM backend. Clang built `libjccbas.a`, linked against UCRT for the JCC LLVM backend. JCC has removed its FASM backend, so no current JCC version uses the DLL. The DLL also blocked a Windows ARM64 release, because no mingw-w64 GCC targets Windows ARM64. The alternative was to keep building the DLL for older JCC versions that still use FASM.

## Decision

We will ship only `libjccbas.a`, built with Clang, on every platform, and remove the GCC/MinGW DLL build. We will do this in libjccbas 3.0.0.

## Consequences

Every platform now has one library, one compiler, and on Windows one C runtime (UCRT). This made it possible to replace the six per-platform makefiles with one `Makefile`, and to add Windows ARM64 to CI and releases. Older JCC versions with the FASM backend cannot use libjccbas 3.0.0 or later. They must stay on libjccbas 2.x. Removing the DLL is an incompatible change to the release archive, which is why the major version changed. JCC still names `libjccbas.dll` in `FunctionUtils.LIB_JCC_BAS`, and that must change in the JCC repo.
