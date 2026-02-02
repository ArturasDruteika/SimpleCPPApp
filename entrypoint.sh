#!/usr/bin/env bash
set -euo pipefail

APP_BIN="${APP_BIN:-/app/bin/service}"

# Start gdbserver by default (you can override by passing a different command)
exec gdbserver --once 0.0.0.0:2000 "${APP_BIN}" "$@"
