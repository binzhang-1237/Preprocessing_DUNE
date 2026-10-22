#!/usr/bin/env bash
set -eo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/root_environment.sh"
# Defaults require no arguments; edit these paths or set environment overrides.
INPUT_FILE="${INPUT_FILE:-$SCRIPT_DIR/../RecoEnergyStudies/RecoEnergyS.root}"
OUTPUT_DIR="${OUTPUT_DIR:-$SCRIPT_DIR/outputs}"
PROGRAM="$SCRIPT_DIR/build/analysis_RecoEnergyStudies_outputs"
[[ -f "$INPUT_FILE" ]] || { echo "Missing input: $INPUT_FILE" >&2; exit 1; }
[[ -x "$PROGRAM" ]] || { echo "Run $SCRIPT_DIR/build_analysis_RecoEnergyStudies_outputs.sh first" >&2; exit 1; }
mkdir -p "$OUTPUT_DIR"
"$PROGRAM" "$INPUT_FILE" "$OUTPUT_DIR" 2>&1 | tee "$OUTPUT_DIR/analysis.log"
