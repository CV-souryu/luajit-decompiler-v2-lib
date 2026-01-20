#!/usr/bin/env bash
set -euo pipefail

BUILD_TYPE=${BUILD_TYPE:-Release}
ARCH=${ARCH:-}

echo "=== Building LuaJITDecode (${BUILD_TYPE}) for $(uname -s) ==="

if [ -n "${ARCH}" ]; then
	echo "Using ARCH=${ARCH}"
	cmake -S . -B build -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_OSX_ARCHITECTURES="${ARCH}"
else
	cmake -S . -B build -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
fi

cmake --build build --parallel
cmake --install build

echo "=== Done! Output in ./dist ==="