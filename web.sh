#!/usr/bin/env sh
# web.sh - Build the Knobby web simulator (WASM)
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$SCRIPT_DIR/config.sh"

if [ -z "$EMCC" ]; then
    echo "ERROR: emcc not found. Install Emscripten (https://emscripten.org/docs/getting_started/downloads.html)" >&2
    exit 1
fi

echo "[INFO] Building web simulator (WASM)..."
$MAKE -C "$SCRIPT_DIR/sim" PYTHON="$PYTHON" LVGL_PATH="$LVGL_PATH" EMCC="$EMCC" web
echo "[OK] Build complete. Run web_real.sh to start the simulator."
