# Knobby Simulator Configuration
# Automatically included by Makefiles.
#
# To override paths for your machine, create a file called config.local.sh
# (gitignored) and set any of these variables there.
# The shell scripts (sim.sh, web.sh, web_real.sh) will pass them to make.
#
# Example config.local.sh:
#   LVGL_PATH=/opt/homebrew/share/arduino/libraries/lvgl
#   CC=clang-17
#   EMCC=/opt/emsdk/upstream/emscripten/emcc

# C compiler — auto-detected by config.sh, passed in from shell
CC        ?= clang

# LVGL path — auto-detected by config.sh, passed in from shell
LVGL_PATH ?= $(HOME)/Documents/Arduino/libraries/lvgl

# Python — auto-detected by config.sh as python3/python
PYTHON    ?= python3

# Emscripten compiler — passed in from shell when available
EMCC      ?= emcc

# Arduino CLI — standard install path (rarely needs changing)
ARDUINO_CLI ?= arduino-cli
