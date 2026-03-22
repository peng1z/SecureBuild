# SecureBuild

[![CI](https://img.shields.io/github/actions/workflow/status/peng/SecureBuild/ci.yml?branch=main)](https://github.com/peng/SecureBuild/actions/workflows/ci.yml)
[![Security](https://img.shields.io/github/actions/workflow/status/peng/SecureBuild/security.yml?branch=main)](https://github.com/peng/SecureBuild/actions/workflows/security.yml)
[![License](https://img.shields.io/github/license/peng/SecureBuild)](./LICENSE)

Production-grade CMake-based C++ library with a security-hardened CI/CD pipeline.

SecureBuild is a small but real tensor and matrix utility library designed to showcase
modern C++ library packaging, containerized builds, and defensive delivery workflows.
The library is intentionally compact; the infrastructure is the main deliverable.

## Architecture

```text
push / pull request / tag
        |
        v
 +--------------------+        +-------------------------+
 |   CMake Configure  | -----> |  Unit Tests (GTest)     |
 +--------------------+        +-------------------------+
        |                                  |
        v                                  v
 +--------------------+        +-------------------------+
 | Docker Multi-Stage  | -----> |  GHCR / Release Assets  |
 +--------------------+        +-------------------------+
        |
        v
 +--------------------+        +-------------------------+
 | Trivy + Syft       | -----> | SARIF + SBOM Artifacts  |
 +--------------------+        +-------------------------+
```

## Library Surface

- Shape validation for tensor-like data
- Element-wise add and multiply
- Dot product for vectors
- Matrix transpose and matrix multiplication

## Build

### Local

```bash
cmake -S . -B build -DBUILD_TESTS=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --install build --prefix build/stage
```

### Docker

```bash
docker build -f docker/Dockerfile --target test -t securebuild:test .
docker run --rm securebuild:test

docker build -f docker/Dockerfile --target release -t securebuild:latest .
docker tag securebuild:latest securebuild:v0.1.0
```

On tagged releases, the workflow publishes both `ghcr.io/peng/securebuild:v0.1.0` and
`ghcr.io/peng/securebuild:latest`.

## Security Practices

SecureBuild treats the pipeline as a first-class artifact.

- CVE scanning uses Trivy against both the container image and the repository filesystem.
- SBOM generation uses Syft in SPDX and CycloneDX formats so downstream tools can ingest the same release.
- GitHub Actions references are pinned to commit SHAs rather than floating tags.
- Dependabot watches both GitHub Actions and Docker base image updates.
- Release jobs refuse to publish if a critical CVE is present in the release image.

## Consume As A Dependency

The library installs a proper CMake package export, so consumers can use `find_package`.

```cmake
cmake_minimum_required(VERSION 3.20)
project(example LANGUAGES CXX)

find_package(SecureBuild CONFIG REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE SecureBuild::securebuild)
```

Example usage in C++:

```cpp
#include "securebuild/tensor_ops.h"

int main() {
  securebuild::Tensor a{{2, 2}, {1.0, 2.0, 3.0, 4.0}};
  securebuild::Tensor b{{2, 2}, {5.0, 6.0, 7.0, 8.0}};
  auto c = securebuild::elementwise_add(a, b);
  return c.values[0] == 6.0 ? 0 : 1;
}
```

## Repository Layout

```text
SecureBuild/
├── .github/
├── cpp/
├── tests/
├── docker/
├── scripts/
├── cmake/
├── README.md
└── LICENSE
```
