# Using libjccbas with Classifiers in JCC

## Overview

The libjccbas library now publishes platform-specific artifacts using Maven classifiers. This allows JCC to automatically pull the correct native library for the build platform.

## Available Artifacts

When deployed, the following artifacts will be available:

```
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:zip:windows-x86_64
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:zip:windows-arm64
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:tar.gz:macos-arm64
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:tar.gz:macos-x86_64
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:tar.gz:linux-arm64
se.dykstrom.jcc:libjccbas:1.5.1-SNAPSHOT:tar.gz:linux-x86_64
```

Note: Windows artifacts (zip files) contain both `libjccbas.dll` (GCC) and `libjccbas.a` (Clang) in the `lib/` directory.

## Integration in JCC pom.xml

Add these profiles to your JCC pom.xml to automatically select the correct classifier:

```xml
<properties>
  <libjccbas.version>1.5.1-SNAPSHOT</libjccbas.version>
</properties>

<profiles>
  <!-- Windows x86_64 -->
  <profile>
    <id>windows-x86_64</id>
    <activation>
      <os>
        <family>windows</family>
        <arch>amd64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>windows-x86_64</classifier>
        <type>zip</type>
      </dependency>
    </dependencies>
  </profile>

  <!-- Windows ARM64 -->
  <profile>
    <id>windows-arm64</id>
    <activation>
      <os>
        <family>windows</family>
        <arch>aarch64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>windows-arm64</classifier>
        <type>zip</type>
      </dependency>
    </dependencies>
  </profile>

  <!-- macOS ARM64 -->
  <profile>
    <id>macos-arm64</id>
    <activation>
      <os>
        <family>mac</family>
        <arch>aarch64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>macos-arm64</classifier>
        <type>tar.gz</type>
      </dependency>
    </dependencies>
  </profile>

  <!-- macOS x86_64 -->
  <profile>
    <id>macos-x86_64</id>
    <activation>
      <os>
        <family>mac</family>
        <arch>x86_64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>macos-x86_64</classifier>
        <type>tar.gz</type>
      </dependency>
    </dependencies>
  </profile>

  <!-- Linux ARM64 -->
  <profile>
    <id>linux-arm64</id>
    <activation>
      <os>
        <family>unix</family>
        <name>Linux</name>
        <arch>aarch64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>linux-arm64</classifier>
        <type>tar.gz</type>
      </dependency>
    </dependencies>
  </profile>

  <!-- Linux x86_64 -->
  <profile>
    <id>linux-x86_64</id>
    <activation>
      <os>
        <family>unix</family>
        <name>Linux</name>
        <arch>amd64</arch>
      </os>
    </activation>
    <dependencies>
      <dependency>
        <groupId>se.dykstrom.jcc</groupId>
        <artifactId>libjccbas</artifactId>
        <version>${libjccbas.version}</version>
        <classifier>linux-x86_64</classifier>
        <type>tar.gz</type>
      </dependency>
    </dependencies>
  </profile>
</profiles>
```

## Extracting the Library

Use the `maven-dependency-plugin` to unpack the artifact during the build:

```xml
<plugin>
  <groupId>org.apache.maven.plugins</groupId>
  <artifactId>maven-dependency-plugin</artifactId>
  <version>3.6.0</version>
  <executions>
    <execution>
      <id>unpack-libjccbas</id>
      <phase>generate-resources</phase>
      <goals>
        <goal>unpack-dependencies</goal>
      </goals>
      <configuration>
        <includeArtifactIds>libjccbas</includeArtifactIds>
        <outputDirectory>${project.build.directory}/libjccbas</outputDirectory>
      </configuration>
    </execution>
  </executions>
</plugin>
```

After unpacking, the library will be available at:
- `${project.build.directory}/libjccbas/lib/libjccbas.dll` (Windows - GCC dynamic library)
- `${project.build.directory}/libjccbas/lib/libjccbas.a` (All platforms - static library)
- `${project.build.directory}/libjccbas/inc/*.h` (all platforms)

Note: Windows artifacts contain both the DLL and static library.

## Deployment

To deploy all platform-specific artifacts to a Maven repository:

```bash
# Build on each platform first (or use CI/CD)
make clean && make

# Then package and deploy
mvn deploy
```

Or use CI/CD to build all platforms and deploy in a single workflow.

## Testing Locally

To test with a local Maven repository before deploying:

```bash
# In libjccbas project
mvn clean install

# In JCC project
# The profile will automatically activate and pull from local ~/.m2/repository
mvn clean package
```
