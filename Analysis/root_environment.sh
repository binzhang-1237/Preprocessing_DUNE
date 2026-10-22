#!/usr/bin/env bash
# Source this file from the build/run scripts; ROOT alone is sufficient.
ROOT_LOCAL="${ROOT_LOCAL:-$HOME/root-local-6.28.12}"
[[ -f "$ROOT_LOCAL/bin/thisroot.sh" ]] || { echo "Missing ROOT: $ROOT_LOCAL" >&2; return 1; }
source "$ROOT_LOCAL/bin/thisroot.sh"
export LD_LIBRARY_PATH="$HOME/local/usr/lib/x86_64-linux-gnu:$HOME/local/jpeg/lib:/usr/lib/x86_64-linux-gnu:$ROOT_LOCAL/lib:${LD_LIBRARY_PATH:-}"
unset PYTHONHOME CPATH C_INCLUDE_PATH CPLUS_INCLUDE_PATH ROOT_INCLUDE_PATH
