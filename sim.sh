#!/usr/bin/env sh
# sim.sh - Launch the native Knobby simulator (PC)
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
. "$SCRIPT_DIR/config.sh"

echo "[INFO] Building and launching native simulator..."
$MAKE -C "$SCRIPT_DIR/sim" CC="$CC" LVGL_PATH="$LVGL_PATH" sim-gui
