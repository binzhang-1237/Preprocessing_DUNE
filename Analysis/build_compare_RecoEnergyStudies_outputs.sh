#!/usr/bin/env bash
set -eo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/root_environment.sh"
mkdir -p "$SCRIPT_DIR/build"
read -r -a ROOT_CFLAGS <<< "$("$ROOT_LOCAL/bin/root-config" --cflags)"
read -r -a ROOT_LIBS <<< "$("$ROOT_LOCAL/bin/root-config" --libs)"
"${CXX:-g++}" "${ROOT_CFLAGS[@]}" -std=c++17 -O2 -Wall -Wextra \
    "$SCRIPT_DIR/compare_RecoEnergyStudies_outputs.cc" "${ROOT_LIBS[@]}" \
    -Wl,-rpath,"$ROOT_LOCAL/lib" -o "$SCRIPT_DIR/build/compare_RecoEnergyStudies_outputs"
printf 'Built: %s/build/compare_RecoEnergyStudies_outputs\n' "$SCRIPT_DIR"
