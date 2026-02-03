#!/usr/bin/env bash
set -euo pipefail

APP_BIN="${APP_BIN:-/app/bin/service}"
GDBSERVER_PORT="${GDBSERVER_PORT:-2000}"

if [ "${DEBUG_WAIT:-1}" = "1" ]; then
    echo "[entrypoint] Starting gdbserver on 0.0.0.0:${GDBSERVER_PORT}"
    echo "[entrypoint] Program: ${APP_BIN} $*"
    exec gdbserver --once "0.0.0.0:${GDBSERVER_PORT}" "${APP_BIN}" "$@"
fi

exec "${APP_BIN}" "$@"
