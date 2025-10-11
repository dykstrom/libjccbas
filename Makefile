# Multi-platform Makefile for jccbasic
# Supports:
#   - Windows x86_64 with GCC (produces jccbasic.dll)
#   - macOS x86_64/arm64 with Clang (produces libjccbas.a)
#   - Linux x86_64/arm64 with Clang (produces libjccbas.a)

# Platform and architecture detection
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
UNAME_M := $(shell uname -m 2>/dev/null || echo x86_64)

ifeq ($(UNAME_S),Darwin)
    PLATFORM=macos
    ARCH=$(UNAME_M)
else ifeq ($(UNAME_S),Linux)
    PLATFORM=linux
    ARCH=$(UNAME_M)
else
    PLATFORM=windows
    ARCH=x86_64
endif

# Include common definitions
include Makefile.common

# Include platform-specific configuration
include Makefile.$(PLATFORM)
