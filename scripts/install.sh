#!/usr/bin/env bash
set -e

PREFIX=${PREFIX:-/usr/local}
BUILD=build

cmake -S . -B $BUILD \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PREFIX"

cmake --build $BUILD
sudo cmake --install $BUILD

