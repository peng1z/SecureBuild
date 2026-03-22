FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
  && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    git \
    ninja-build \
    pkg-config \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . .

ARG BUILD_TYPE=Release

RUN cmake -S . -B build -G Ninja \
  -DBUILD_TESTS=ON \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DCMAKE_INSTALL_PREFIX=/opt/securebuild

RUN cmake --build build --parallel
RUN cmake --install build

FROM builder AS test

RUN ctest --test-dir build --output-on-failure

CMD ["ctest", "--test-dir", "build", "--output-on-failure"]

FROM scratch AS release

LABEL org.opencontainers.image.title="SecureBuild"
LABEL org.opencontainers.image.description="SecureBuild static C++ library distribution"

COPY --from=builder /opt/securebuild /opt/securebuild

ENV SecureBuild_DIR=/opt/securebuild/lib/cmake/SecureBuild
ENV SecureBuild_ROOT=/opt/securebuild

WORKDIR /opt/securebuild
