#!/usr/bin/env bash
set -euo pipefail

APP_BIN="/app/build/SimpleCPPApp"
BUILD_DIR="/app/build"

# Configure only once (or if build system files are missing)
if [ ! -f "${BUILD_DIR}/build.ninja" ] && [ ! -f "${BUILD_DIR}/Makefile" ]; then
  mkdir -p "${BUILD_DIR}"
  cmake -S /app -B "${BUILD_DIR}" -G Ninja -DCMAKE_BUILD_TYPE=Debug
fi

# Incremental build (fast if nothing changed)
cmake --build "${BUILD_DIR}"

# Start gdbserver by default (you can override by passing a different command)
exec gdbserver --once 0.0.0.0:2000 "${APP_BIN}" "$@"
