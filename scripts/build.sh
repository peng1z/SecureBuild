#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
build_dir="${BUILD_DIR:-${repo_root}/build}"
build_type="${BUILD_TYPE:-Release}"
shared_libs="${BUILD_SHARED_LIBS:-OFF}"

cmake -S "${repo_root}" -B "${build_dir}" \
  -DBUILD_TESTS=ON \
  -DBUILD_SHARED_LIBS="${shared_libs}" \
  -DCMAKE_BUILD_TYPE="${build_type}"

cmake --build "${build_dir}" --parallel
ctest --test-dir "${build_dir}" --output-on-failure
