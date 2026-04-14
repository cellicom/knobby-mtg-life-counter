#!/usr/bin/env sh
# sim-web.sh - Launch the real Knobby web simulator in browser
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export VERBOSE="${VERBOSE:-1}"
. "$SCRIPT_DIR/config.sh"

if [ ! -f "$SCRIPT_DIR/sim/knobby_web.wasm" ]; then
    echo "[ERROR] knobby_web.wasm not found. Run ./compile.sh first to compile." >&2
    exit 1
fi

PORT="${PORT:-8000}"
URL="http://localhost:$PORT/sim/index.html"
echo "[INFO] Starting local web server on $URL"
echo "[INFO] Press Ctrl+C to stop."

# Open browser in background (cross-platform)
OS="$(uname -s 2>/dev/null || echo Windows)"
case "$OS" in
    Linux*)   command -v xdg-open >/dev/null && xdg-open "$URL" & ;;
    Darwin*)  open "$URL" & ;;
    *)        start "" "$URL" 2>/dev/null || true ;;
esac

cd "$SCRIPT_DIR"
$PYTHON -m http.server "$PORT"
