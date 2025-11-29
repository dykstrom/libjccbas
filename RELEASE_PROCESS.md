# Release Process

This document describes how to create releases for libjccbas using the Maven Release Plugin, which automates version management, tagging, and triggers the GitHub Actions release workflow.

## Quick Start

```bash
# Ensure clean state on master
git checkout master
git pull origin master
git status  # Should be clean

# Create release (interactive)
mvn release:prepare

# Monitor GitHub Actions for build/release
```

## Overview

The release process consists of two parts:

1. **Maven Release Plugin**: Automates version updates, Git commits, and tag creation
2. **GitHub Actions**: Automatically builds on all platforms (Windows/macOS/Linux x86_64, macOS/Linux ARM64), runs tests, and creates GitHub Releases with artifacts

When you run `mvn release:prepare`, it automatically:
- Updates `pom.xml` to release version (removes `-SNAPSHOT`)
- Commits with message: `[maven-release-plugin] prepare release vX.Y.Z`
- Creates and pushes annotated tag `vX.Y.Z`
- Updates `pom.xml` to next development version (adds `-SNAPSHOT`)
- Commits with message: `[maven-release-plugin] prepare for next development iteration`

The pushed tag triggers GitHub Actions to build, test, and release on all platforms.

## Prerequisites

Before releasing, ensure:

1. **On master branch**: Releases must originate from `master`
   ```bash
   git checkout master
   git pull origin master
   ```

2. **Clean working directory**: No uncommitted changes
   ```bash
   git status  # Should show "nothing to commit, working tree clean"
   ```

3. **Git authentication**: Can push to repository
   ```bash
   git remote -v  # Verify remote URL
   ```

4. **Tests pass locally**: Run the test suite
   ```bash
   ./run_all_tests.sh
   ```

## Creating a Release

### Interactive Mode (Recommended)

Run the release plugin:

```bash
mvn release:prepare
```

You'll be prompted for:

1. **Release version** (e.g., `2.0.1`)
   - Default removes `-SNAPSHOT` from current version

2. **Tag name** (e.g., `v2.0.1`)
   - Automatically prefixed with `v` via configured `tagNameFormat`
   - Default is `v` + release version

3. **Next development version** (e.g., `2.0.2-SNAPSHOT`)
   - Default increments patch version and adds `-SNAPSHOT`

Press Enter to accept defaults or type custom values.

### Non-Interactive Mode

For automation or when you know the versions:

```bash
mvn release:prepare -B \
  -DreleaseVersion=2.0.1 \
  -Dtag=v2.0.1 \
  -DdevelopmentVersion=2.0.2-SNAPSHOT
```

The `-B` flag runs in batch mode without prompts.

### Monitoring the Build

1. Navigate to the **Actions** tab in GitHub
2. Find the **release** workflow triggered by your tag
3. Monitor build progress for all platforms
4. Once complete, check the **Releases** tab for platform-specific artifacts:
   - `libjccbas-X.Y.Z-windows-x86_64.zip`
   - `libjccbas-X.Y.Z-macos-arm64.tar.gz`
   - `libjccbas-X.Y.Z-macos-x86_64.tar.gz`
   - `libjccbas-X.Y.Z-linux-x86_64.tar.gz`
   - `libjccbas-X.Y.Z-linux-arm64.tar.gz`

## Testing Before Release

Test the release process without making changes:

```bash
# Dry run
mvn release:prepare -DdryRun=true

# Review output, then clean up
mvn release:clean
```

## Troubleshooting

### Working Directory Not Clean

```bash
git status
git stash  # If you have local changes to preserve
```

### Release Fails After Tag Push

If the GitHub Actions workflow fails:

1. Check workflow logs for errors
2. Fix the issue in code
3. Delete the tag locally and remotely:
   ```bash
   git tag -d vX.Y.Z
   git push origin :refs/tags/vX.Y.Z
   ```
4. Reset version changes:
   ```bash
   git reset --hard origin/master
   ```
5. Re-run `mvn release:prepare`

### Tag Not on Master Branch

If you accidentally ran the release from a non-master branch:

1. Delete the tag:
   ```bash
   git push origin :refs/tags/vX.Y.Z
   ```

2. Switch to master:
   ```bash
   git checkout master
   git pull origin master
   ```

3. Re-run release:
   ```bash
   mvn release:rollback  # If needed
   mvn release:clean
   mvn release:prepare
   ```

### Rolling Back Before Push

If `release:prepare` fails before pushing:

```bash
mvn release:rollback
```

This removes the tag and reverts version changes. Only works if changes haven't been pushed yet.

### Cleaning Up

After successful or failed release:

```bash
mvn release:clean
```

Removes temporary files: `release.properties`, `pom.xml.releaseBackup`, etc.

## Manual Release Process

For special cases (hotfixes, custom workflows), you can create releases manually:

### Step 1: Update Version

Edit `pom.xml`:

```xml
<version>2.0.1</version>  <!-- Remove -SNAPSHOT -->
```

Commit:

```bash
git add pom.xml
git commit -m "Bump version to 2.0.1 for release"
git push origin master
```

### Step 2: Create and Push Tag

```bash
git tag -a v2.0.1 -m "Release version 2.0.1"
git push origin v2.0.1
```

Tag format must be `v<major>.<minor>.<patch>`.

### Step 3: Update to Development Version

Edit `pom.xml`:

```xml
<version>2.0.2-SNAPSHOT</version>
```

Commit:

```bash
git add pom.xml
git commit -m "Bump version to 2.0.2-SNAPSHOT for development"
git push origin master
```

## Version Numbering

This project follows [Semantic Versioning](https://semver.org/):

- **MAJOR**: Incompatible API changes
- **MINOR**: New functionality, backwards compatible
- **PATCH**: Backwards compatible bug fixes

Development versions use `-SNAPSHOT` suffix (e.g., `2.0.2-SNAPSHOT`).

## Release Artifacts

Each platform-specific archive contains:

- `lib/` - Native library files
  - Windows: `libjccbas.dll` (GCC), `libjccbas.a` (Clang)
  - macOS/Linux: `libjccbas.a` (Clang)
- `inc/` - C header files for all BASIC functions
- `README.md` - Project documentation
- `LICENSE` - License information

## Workflow Configuration

The release workflow (`.github/workflows/release.yml`) includes:

- **Trigger**: Tags matching `v*` pattern (must be on master branch)
- **Platforms**: Windows x86_64, macOS ARM64, macOS x86_64, Linux x86_64, Linux ARM64
- **Jobs**:
  1. `verify-branch`: Verifies tag points to master branch commit
  2. `build-and-test`: Builds and tests on all platforms (parallel)
  3. `create-release`: Creates GitHub Release with all artifacts

Artifacts are retained for 90 days.

## Integration with JCC

Released artifacts are designed for the [JCC compiler](https://github.com/dykstrom/jcc). Download the appropriate platform-specific archive and follow JCC's integration documentation.
