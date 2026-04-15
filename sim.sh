#!/usr/bin/env sh
# sim.sh - Launch the native Knobby simulator (PC)
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export VERBOSE="${VERBOSE:-1}"
. "$SCRIPT_DIR/config.sh"

echo "[1/2] Compiling native simulator..."
$MAKE -C "$SCRIPT_DIR/sim" CC="$CC" LVGL_PATH="$LVGL_PATH" sim-gui

echo "[2/2] Launching simulator..."
"$SCRIPT_DIR/sim/knobby_sim_gui.exe"
