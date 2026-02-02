#!/usr/bin/env bash
set -euo pipefail

APP_BIN="${APP_BIN:-/app/bin/service}"

if [ "${DEBUG_WAIT:-1}" = "1" ]; then
	exec sleep infinity
fi

exec "${APP_BIN}" "$@"
