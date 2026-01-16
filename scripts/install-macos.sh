#!/usr/bin/env bash
set -e

PREFIX=${PREFIX:-/usr/local}
BUILD=build
BIN_DIR="$PREFIX/bin"

cmake -S . -B "$BUILD" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD"

# Install binary
sudo install -m 755 "$BUILD/src/ykushcmd" "$BIN_DIR/ykushcmd"

# Install hidapi dylib next to the binary
sudo install -m 755 "$BUILD/hidapi/src/mac/libhidapi."*.dylib "$BIN_DIR"

# Ensure the binary can find dylibs in its own directory
sudo install_name_tool -add_rpath "@executable_path" "$BIN_DIR/ykushcmd"

