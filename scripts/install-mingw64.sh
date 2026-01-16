#!/usr/bin/env bash
set -e

BUILD=build-mingw64
STAGING=${STAGING:-$(pwd)/dist/windows}

rm -rf "$BUILD" "$STAGING"

cmake -S . -B "$BUILD" \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

cmake --build "$BUILD"
cmake --install "$BUILD" --prefix "$STAGING"

