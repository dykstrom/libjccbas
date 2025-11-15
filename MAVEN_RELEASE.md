# Using Maven Release Plugin

This guide explains how to use the Maven Release Plugin to automate the release process for libjccbas.

## Overview

The Maven Release Plugin can automate the manual steps described in `RELEASE_PROCESS.md`:
- Update version numbers in `pom.xml`
- Create Git commits for version changes
- Create and push Git tags
- Automatically trigger the GitHub Actions release workflow

## Prerequisites

1. **Git Authentication**: Ensure you can push to the repository
   ```bash
   git remote -v  # Verify remote URL
   ```

2. **On Master Branch**: You must be on the `master` branch
   ```bash
   git checkout master
   git pull origin master
   ```

3. **Clean Working Directory**: No uncommitted changes
   ```bash
   git status  # Should show "nothing to commit, working tree clean"
   ```

4. **Maven Installed**: Maven 3.x
   ```bash
   mvn --version
   ```

## Creating a Release (Interactive Mode)

### Step 1: Prepare the Release

```bash
mvn release:prepare
```

The plugin will prompt you for:

1. **Release version**: The version to release (e.g., `1.5.1`)
   - Default removes `-SNAPSHOT` from current version
   - Press Enter to accept default or type a different version

2. **Tag name**: The Git tag to create (e.g., `v1.5.1`)
   - The `v` prefix is automatically added via the configured `tagNameFormat`
   - Default will be `v` + release version (e.g., if version is `1.5.1`, tag will be `v1.5.1`)
   - Press Enter to accept default or type a different tag name

3. **Next development version**: The next SNAPSHOT version (e.g., `1.5.2-SNAPSHOT`)
   - Default increments patch version and adds `-SNAPSHOT`
   - Press Enter to accept or type a different version

### Step 2: Verify and Push

The plugin automatically:
- ✓ Updates `pom.xml` to release version (1.5.1)
- ✓ Commits with message: `[maven-release-plugin] prepare release v1.5.1`
- ✓ Creates annotated tag `v1.5.1`
- ✓ Updates `pom.xml` to next development version (1.5.2-SNAPSHOT)
- ✓ Commits with message: `[maven-release-plugin] prepare for next development iteration`
- ✓ Pushes commits and tag to GitHub

### Step 3: Monitor GitHub Actions

1. Navigate to **Actions** tab in GitHub
2. Find the **release** workflow triggered by tag `v1.5.1`
3. Monitor the build across all platforms
4. Once complete, check the **Releases** tab for your new release

## Creating a Release (Non-Interactive Mode)

For automation or CI/CD:

```bash
mvn release:prepare -B \
  -DreleaseVersion=1.5.1 \
  -Dtag=v1.5.1 \
  -DdevelopmentVersion=1.5.2-SNAPSHOT
```

The `-B` flag runs in batch mode (no prompts).

## Dry Run (Testing Without Changes)

To test the release process without making actual changes:

```bash
mvn release:prepare -DdryRun=true
```

This simulates the release process and shows what would happen. Review the output, then clean up:

```bash
mvn release:clean
```

## Rolling Back a Failed Release

If something goes wrong during `release:prepare`:

```bash
mvn release:rollback
```

This will:
- Remove the tag (if created)
- Revert version changes in `pom.xml`
- Reset to the state before `release:prepare`

**Note**: This only works if you haven't pushed yet. If already pushed, see troubleshooting below.

## Cleaning Up Release Files

After a successful or failed release, clean up temporary files:

```bash
mvn release:clean
```

This removes:
- `release.properties`
- `pom.xml.releaseBackup`
- Other temporary files created by the plugin

## Advanced Configuration

### Tag Format

The Maven Release Plugin is already configured with a custom tag format in `pom.xml`:

```xml
<configuration>
  <tagNameFormat>v@{project.version}</tagNameFormat>
</configuration>
```

This automatically adds the `v` prefix to tags (e.g., version `1.5.1` → tag `v1.5.1`), which is required by the GitHub Actions workflow.

### Skip Tests During Prepare

If you've already run tests locally:

```bash
mvn release:prepare -Darguments="-DskipTests"
```

### Push to Specific Remote

If you have multiple remotes:

```bash
mvn release:prepare -DpushChanges=true -DremoteTagging=true
```

## Troubleshooting

### Release Plugin Says "Working Directory Not Clean"

Ensure no uncommitted changes:
```bash
git status
git stash  # If you have local changes
```

### Wrong Tag Name Created

If you accidentally created a tag without the `v` prefix:

1. Rollback if not pushed:
   ```bash
   mvn release:rollback
   ```

2. If already pushed, manually fix:
   ```bash
   # Delete bad tag
   git tag -d 1.5.1
   git push origin :refs/tags/1.5.1

   # Reset to before release commits
   git reset --hard HEAD~2
   git push origin master --force

   # Start over with correct tag name
   mvn release:prepare
   ```

### Authentication Failures

If Git push fails:

1. **HTTPS**: Check credentials
   ```bash
   git config --global credential.helper store
   ```

2. **SSH**: Ensure SSH key is added to GitHub
   ```bash
   ssh -T git@github.com
   ```

3. **Token**: Use personal access token with `repo` scope

### Release Workflow Fails "Not on Master"

This means the tag was created from a non-master branch:

1. Delete the tag:
   ```bash
   git push origin :refs/tags/v1.5.1
   ```

2. Ensure you're on master:
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

## Comparison: Maven Plugin vs Manual Process

| Aspect | Maven Release Plugin | Manual Process |
|--------|---------------------|----------------|
| Version updates | Automatic | Manual editing |
| Git commits | Automatic | Manual commits |
| Tag creation | Automatic | Manual tagging |
| Push to GitHub | Automatic | Manual push |
| Error-prone | Lower | Higher |
| Rollback | Built-in | Manual |
| Learning curve | Medium | Low |
| Customization | High (via config) | Full control |

## Recommended Workflow

For most releases, use Maven Release Plugin:

```bash
# 1. Ensure clean state on master
git checkout master
git pull origin master
git status

# 2. Optional: Dry run first
mvn release:prepare -DdryRun=true
mvn release:clean

# 3. Create release
mvn release:prepare

# 4. Monitor GitHub Actions for build/release
```

For special situations (hotfixes, beta releases), use the manual process described in `RELEASE_PROCESS.md`.

## Integration with GitHub Actions

The Maven Release Plugin creates and pushes tags that automatically trigger the GitHub Actions release workflow:

```
mvn release:prepare
       ↓
Creates & pushes v1.5.1 tag
       ↓
GitHub Actions detects tag
       ↓
verify-branch (checks master)
       ↓
build-and-test (all platforms)
       ↓
create-release (GitHub Release)
```

The actual platform-specific builds and GitHub Release creation are handled entirely by GitHub Actions. Maven Release Plugin only manages version control and tagging.
