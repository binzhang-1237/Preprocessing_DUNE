#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PYTHON="${PREPROCESS2_PYTHON:-/mnt/ironwolf_14t2/users/binzhang/local/dune-preprocessing/bin/python}"
INPUT_FILE="${1:-$SCRIPT_DIR/../dune_pixelmap_generator/output/pixel_maps_test.h5}"
OUTPUT_FILE="${2:-$SCRIPT_DIR/output/preprocessed_$(basename -- "$INPUT_FILE")}"

[[ -x "$PYTHON" ]] || { echo "Missing Python environment: $PYTHON" >&2; exit 1; }
[[ -f "$INPUT_FILE" ]] || { echo "Missing input: $INPUT_FILE" >&2; exit 1; }
mkdir -p -- "$(dirname -- "$OUTPUT_FILE")"

printf 'Python=%s\nInput=%s\nOutput=%s\n' "$PYTHON" "$INPUT_FILE" "$OUTPUT_FILE"
exec "$PYTHON" -I "$SCRIPT_DIR/preprocess2.py" "$OUTPUT_FILE" "$INPUT_FILE"
