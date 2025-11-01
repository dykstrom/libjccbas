# Distribution Notes

## Platform-Specific Maven Artifacts

The libjccbas library now uses Maven profiles to build platform-specific artifacts with classifiers.

### Key Features

1. **Profile-based builds** - Only the artifact for the current platform is built when running `mvn package`
2. **Classifier-based artifacts** - Each platform/toolchain combination gets its own classifier
3. **Consistent format** - tar.gz for Unix-like systems (macOS, Linux), zip for Windows

### Artifacts Produced

When built on different platforms, the following artifacts are created:

| Platform | Artifact | Classifier | Type | Library Files |
|----------|----------|------------|------|---------------|
| Windows x86_64 | libjccbas-VERSION-windows-x86_64.zip | windows-x86_64 | zip | libjccbas.dll + libjccbas.a |
| Windows ARM64 | libjccbas-VERSION-windows-arm64.zip | windows-arm64 | zip | libjccbas.dll + libjccbas.a |
| macOS ARM64 | libjccbas-VERSION-macos-arm64.tar.gz | macos-arm64 | tar.gz | libjccbas.a |
| macOS x86_64 | libjccbas-VERSION-macos-x86_64.tar.gz | macos-x86_64 | tar.gz | libjccbas.a |
| Linux ARM64 | libjccbas-VERSION-linux-arm64.tar.gz | linux-arm64 | tar.gz | libjccbas.a |
| Linux x86_64 | libjccbas-VERSION-linux-x86_64.tar.gz | linux-x86_64 | tar.gz | libjccbas.a |

Note: Windows artifacts contain both the GCC dynamic library (libjccbas.dll) and the Clang static library (libjccbas.a).

### Profile Activation

Profiles are automatically activated based on OS detection:

- **windows-x86_64**: Windows + amd64
- **windows-arm64**: Windows + aarch64
- **macos-arm64**: macOS + aarch64
- **macos-x86_64**: macOS + x86_64
- **linux-arm64**: Linux + aarch64
- **linux-x86_64**: Linux + amd64

### Build Workflow

#### Local Development

```bash
# Build the native library
make clean && make

# Package for current platform
mvn clean package

# Install to local Maven repository
mvn clean install
```

#### Multi-Platform Release

For releasing all platform variants:

1. Build on each platform:
   ```bash
   # On Windows x86_64
   make clean && make
   mvn clean package  # Creates windows-x86_64.zip

   # On Windows ARM64
   make clean && make
   mvn clean package  # Creates windows-arm64.zip

   # On macOS ARM64
   make clean && make
   mvn clean package  # Creates macos-arm64.tar.gz

   # On macOS x86_64 (or use CI/CD)
   make clean && make
   mvn clean package  # Creates macos-x86_64.tar.gz

   # On Linux ARM64
   make clean && make
   mvn clean package  # Creates linux-arm64.tar.gz

   # On Linux x86_64
   make clean && make
   mvn clean package  # Creates linux-x86_64.tar.gz
   ```

2. Deploy all artifacts:
   ```bash
   # From each platform, or collect all artifacts and deploy from one location
   mvn deploy
   ```

#### Using GitHub Actions (Implemented)

The repository includes a GitHub Actions workflow (`.github/workflows/build.yml`) that automatically builds and packages the library for multiple platforms.

**Supported platforms:**
- Windows x86_64 (using `windows-latest`)
- macOS ARM64 (using `macos-14` - Apple Silicon)
- macOS x86_64 (using `macos-13` - Intel)
- Linux x86_64 (using `ubuntu-latest`)

**Workflow features:**
- Matrix build across 4 platforms
- Automatic toolchain installation:
  - Windows: MinGW-w64 (GCC) and LLVM (Clang) via Chocolatey
  - Linux: Clang and LLVM via apt
  - macOS: Pre-installed Clang from Xcode Command Line Tools
- Runs full build and test suite
- Creates Maven packages
- Uploads platform-specific artifacts with 30-day retention

**Triggers:**
- Push to `master` or `dev` branches
- Pull requests to `master`
- Manual workflow dispatch

**Windows ARM64 note:**
Not included in the matrix because GitHub doesn't provide native ARM64 Windows runners. Would require a self-hosted runner or cross-compilation setup.

**Artifacts:**
After each workflow run, platform-specific packages are available as artifacts:
- `libjccbas-windows-x86_64` (contains .zip)
- `libjccbas-macos-arm64` (contains .tar.gz)
- `libjccbas-macos-x86_64` (contains .tar.gz)
- `libjccbas-linux-x86_64` (contains .tar.gz)

### Integration in JCC

See `USAGE_IN_JCC.md` for complete integration instructions.

The JCC project should:
1. Add the same profile structure with dependencies on libjccbas artifacts
2. Use `maven-dependency-plugin` to unpack the artifact during build
3. Reference the unpacked library and headers in integration tests and distribution

### Archive Contents

Each artifact contains:
```
libjccbas-VERSION/
├── LICENSE
├── README.md
├── lib/
│   └── libjccbas.dll or libjccbas.a
└── inc/
    └── *.h (all header files)
```

### Testing

To verify the package contains the correct libraries:

```bash
# For tar.gz (macOS/Linux)
tar -tzf target/libjccbas-*-macos-arm64.tar.gz | grep "lib/"
# Expected: lib/libjccbas.a

# For zip (Windows)
unzip -l target/libjccbas-*-windows-x86_64.zip | grep "lib/"
# Expected: lib/libjccbas.dll and lib/libjccbas.a
```
