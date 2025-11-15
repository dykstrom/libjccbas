# Multi-platform Makefile for libjccbas
# Supports:
#   - Windows x86_64 with GCC (produces libjccbas.dll) and Clang (produces libjccbas.a)
#   - macOS x86_64/arm64 with Clang (produces libjccbas.a)
#   - Linux x86_64/arm64 with Clang (produces libjccbas.a)

# Platform and architecture detection
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
UNAME_M := $(shell uname -m 2>/dev/null || echo x86_64)
UNAME_A := $(shell uname -a 2>/dev/null || echo)

ifeq ($(UNAME_S),Darwin)
    PLATFORM=macos
    ARCH=$(UNAME_M)
else ifeq ($(UNAME_S),Linux)
    PLATFORM=linux
    ARCH=$(UNAME_M)
else
    PLATFORM=windows
    # On Windows, detect actual hardware architecture
    # Check multiple sources as emulation layers can mask the real architecture:
    # 1. PROCESSOR_IDENTIFIER - contains actual CPU info (e.g., "ARMv8 (64-bit)")
    # 2. uname -a output - may contain ARM64 marker (e.g., MINGW64_NT-10.0-26100-ARM64)
    # 3. PROCESSOR_ARCHITECTURE - may show AMD64 even on ARM64 when running in emulation
    ifneq (,$(findstring ARMv8,$(PROCESSOR_IDENTIFIER)))
        ARCH=aarch64
    else ifneq (,$(findstring ARM64,$(UNAME_A)))
        ARCH=aarch64
    else ifneq (,$(findstring ARM64,$(PROCESSOR_ARCHITECTURE)))
        ARCH=aarch64
    else
        ARCH=x86_64
    endif
endif

# Include common definitions
include Makefile.common

# Include platform-specific configuration
include Makefile.$(PLATFORM)
