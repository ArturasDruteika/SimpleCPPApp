#!/usr/bin/env bash
set -euo pipefail

APP_BIN="${APP_BIN:-/app/bin/service}"
GDBSERVER_PORT="${GDBSERVER_PORT:-2000}"
DEBUG_TRANSPORT="${DEBUG_TRANSPORT:-gdbserver}"

if [ "${DEBUG_WAIT:-1}" = "1" ]; then
    case "${DEBUG_TRANSPORT}" in
        gdbserver)
            echo "[entrypoint] Starting gdbserver on 0.0.0.0:${GDBSERVER_PORT}"
            echo "[entrypoint] Program: ${APP_BIN} $*"
            exec gdbserver --once "0.0.0.0:${GDBSERVER_PORT}" "${APP_BIN}" "$@"
            ;;
        pipetransport)
            echo "[entrypoint] Waiting for pipe transport (sleeping)"
            exec sleep infinity
            ;;
        *)
            echo "[entrypoint] Unknown DEBUG_TRANSPORT: ${DEBUG_TRANSPORT}"
            exit 1
            ;;
    esac
fi

exec "${APP_BIN}" "$@"
