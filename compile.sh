#!/usr/bin/env sh
# compile.sh - Build the Knobby web simulator (WASM)
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export VERBOSE="${VERBOSE:-1}"
. "$SCRIPT_DIR/config.sh"

if [ -z "$EMCC" ]; then
    echo "[ERROR] emcc not found. Install Emscripten (https://emscripten.org/docs/getting_started/downloads.html)" >&2
    exit 1
fi

echo "[1/2] Compiling web simulator (WASM)..."
$MAKE -C "$SCRIPT_DIR/sim" PYTHON="$PYTHON" LVGL_PATH="$LVGL_PATH" EMCC="$EMCC" web

echo "[2/2] Verifying output..."
if [ -f "$SCRIPT_DIR/sim/knobby_web.wasm" ]; then
    echo "[OK] Build complete: sim/knobby_web.wasm (~$(du -h "$SCRIPT_DIR/sim/knobby_web.wasm" | cut -f1))"
    echo "[INFO] Run ./sim-web.sh to start the simulator."
else
    echo "[ERROR] Build failed: knobby_web.wasm not found."
    exit 1
fi
