# Release Process

This document describes the manual release process for libjccbas using GitHub Actions.

**Alternative**: For automated releases, see `MAVEN_RELEASE.md` which describes using the Maven Release Plugin to automate version management and tag creation.

## Overview

The release workflow is automated through GitHub Actions and triggers when a version tag is pushed to the repository. The workflow:

1. Builds the library on all supported platforms (Windows x86_64, macOS ARM64, macOS x86_64, Linux x86_64)
2. Runs the complete test suite on each platform
3. Packages the libraries using Maven
4. Creates a GitHub Release with all platform-specific artifacts

## Prerequisites

Before creating a release, ensure:

1. All changes are committed and pushed to the `master` branch
2. The version number in `pom.xml` matches the intended release version
3. All tests pass locally: `./run_all_tests.sh`
4. The build succeeds on all platforms (verify in GitHub Actions)

## Creating a Release

**IMPORTANT:** Releases can only be created from the `master` branch. The workflow will automatically verify that any version tag points to a commit on the master branch before proceeding with the build.

### Step 1: Update Version Number

Edit `pom.xml` and update the version:

```xml
<version>1.5.1</version>  <!-- Remove -SNAPSHOT for release -->
```

Commit the version change:

```bash
git add pom.xml
git commit -m "Bump version to 1.5.1 for release"
git push origin master
```

### Step 2: Create and Push a Version Tag

Create an annotated tag with a 'v' prefix:

```bash
git tag -a v1.5.1 -m "Release version 1.5.1"
git push origin v1.5.1
```

The tag format must be `v<major>.<minor>.<patch>` (e.g., `v1.5.1`, `v2.0.0`).

### Step 3: Monitor the Release Workflow

1. Navigate to the **Actions** tab in GitHub
2. Find the **release** workflow run triggered by your tag
3. Monitor the build progress for all platforms
4. Each platform build includes:
   - Toolchain installation and verification
   - Library compilation (Make build)
   - Test execution (`run_all_tests.sh`)
   - Maven packaging

### Step 4: Verify the Release

Once the workflow completes successfully:

1. Navigate to the **Releases** tab in GitHub
2. Find the newly created release (e.g., "Release 1.5.1")
3. Verify all platform artifacts are attached:
   - `libjccbas-1.5.1-windows-x86_64.zip`
   - `libjccbas-1.5.1-macos-arm64.tar.gz`
   - `libjccbas-1.5.1-macos-x86_64.tar.gz`
   - `libjccbas-1.5.1-linux-x86_64.tar.gz`

### Step 5: Update to Next Development Version

After the release, update `pom.xml` to the next development version:

```xml
<version>1.5.2-SNAPSHOT</version>  <!-- Next development version -->
```

Commit and push:

```bash
git add pom.xml
git commit -m "Bump version to 1.5.2-SNAPSHOT for development"
git push origin master
```

## Release Artifacts

Each platform-specific archive contains:

- `lib/` - Native library files
  - Windows: `libjccbas.dll` (GCC) and `libjccbas.a` (Clang)
  - macOS/Linux: `libjccbas.a` (Clang)
- `inc/` - C header files for all BASIC functions
- `README.md` - Project documentation
- `LICENSE` - License information

## Troubleshooting

### Tag Not on Master Branch

If you accidentally push a tag from a non-master branch (e.g., `dev`), the workflow will fail at the verification step with:

```
✗ Tag is NOT on master branch
```

To fix this:

1. Delete the tag locally and remotely:
   ```bash
   git tag -d v1.5.1
   git push origin :refs/tags/v1.5.1
   ```

2. Switch to master and ensure it's up to date:
   ```bash
   git checkout master
   git pull origin master
   ```

3. Create the tag on master:
   ```bash
   git tag -a v1.5.1 -m "Release version 1.5.1"
   git push origin v1.5.1
   ```

### Build Fails on a Platform

If the build fails on a specific platform:

1. Check the workflow logs for that platform
2. Fix the issue in the code
3. Delete the tag: `git tag -d v1.5.1 && git push origin :refs/tags/v1.5.1`
4. Commit the fixes
5. Recreate the tag and push it again

### Tests Fail

If tests fail during the release:

1. Review the test output in the workflow logs
2. Fix the failing tests
3. Follow the "Build Fails on a Platform" procedure above

### Release Creation Fails

If the release creation step fails but all builds succeeded:

1. Check if a release with that tag already exists (delete it if needed)
2. Verify the `GITHUB_TOKEN` has sufficient permissions
3. Re-run the workflow from the Actions tab

## Manual Release Trigger

The release workflow can be triggered manually for testing:

1. Navigate to **Actions** → **release** workflow
2. Click **Run workflow**
3. Select the branch and click **Run workflow**

Note: Manual triggers will attempt to extract version from the current branch/tag context.

## Version Numbering

This project follows [Semantic Versioning](https://semver.org/):

- **MAJOR** version for incompatible API changes
- **MINOR** version for new functionality in a backwards compatible manner
- **PATCH** version for backwards compatible bug fixes

Development versions use the `-SNAPSHOT` suffix (e.g., `1.5.2-SNAPSHOT`).

## Workflow Configuration

The release workflow is defined in `.github/workflows/release.yml` and includes:

- **Trigger**: Pushes to tags matching `v*` pattern (must be on master branch)
- **Platforms**: Windows x86_64, macOS ARM64, macOS x86_64, Linux x86_64
- **Jobs**:
  1. `verify-branch`: Verifies the tag points to a commit on the master branch
  2. `build-and-test`: Builds and tests on all platforms in parallel (only runs if verification passes)
  3. `create-release`: Creates GitHub Release with all artifacts after successful builds
- **Artifacts**: Retained for 90 days (longer than regular build artifacts)
- **Branch Restriction**: Only tags on the master branch will trigger a release build

## Integration with JCC

Released artifacts are designed for integration with the [JCC compiler](https://github.com/dykstrom/jcc). Users of JCC should download the appropriate platform-specific archive and follow JCC's integration documentation.
