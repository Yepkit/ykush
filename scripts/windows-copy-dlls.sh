#!/usr/bin/env bash
set -e

DEST=${1:-bin}
PREFIX=/usr/x86_64-w64-mingw32/bin

DLLS=(
  libhidapi-0.dll
  libgcc_s_seh-1.dll
  libstdc++-6.dll
  libwinpthread-1.dll
)

for d in "${DLLS[@]}"; do
  cp "$PREFIX/$d" "$DEST/" 2>/dev/null || true
done

