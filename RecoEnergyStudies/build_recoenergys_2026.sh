#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Reuse the existing build procedure with the 2026 LArSoft release.
export LARSOFT_VERSION="v10_20_09"
export LARSOFT_WORK="${LARSOFT_WORK:-$HOME/disk_binzhang/larsoft_v10_20_09}"

exec "$SCRIPT_DIR/build_recoenergys.sh" "$@"
