#!/usr/bin/env bash
set -eo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/root_environment.sh"
INPUT_2018="${1:-$SCRIPT_DIR/outputs_2018/RecoEnergyStudies_histograms.root}"
INPUT_2026="${2:-$SCRIPT_DIR/outputs_2026/RecoEnergyStudies_histograms.root}"
OUTPUT_DIR="${3:-$SCRIPT_DIR/comparison_2018_vs_2026}"
PROGRAM="$SCRIPT_DIR/build/compare_RecoEnergyStudies_outputs"
[[ -f "$INPUT_2018" ]] || { echo "Missing input: $INPUT_2018" >&2; exit 1; }
[[ -f "$INPUT_2026" ]] || { echo "Missing input: $INPUT_2026" >&2; exit 1; }
if [[ ! -x "$PROGRAM" || "$SCRIPT_DIR/compare_RecoEnergyStudies_outputs.cc" -nt "$PROGRAM" ]]; then
    "$SCRIPT_DIR/build_compare_RecoEnergyStudies_outputs.sh"
fi
mkdir -p "$OUTPUT_DIR"
"$PROGRAM" "$INPUT_2018" "$INPUT_2026" "$OUTPUT_DIR" 2>&1 | tee "$OUTPUT_DIR/comparison.log"
