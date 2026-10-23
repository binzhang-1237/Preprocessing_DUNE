#!/usr/bin/env bash
set -eo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/root_environment.sh"
# Optional arguments: an input directory (all *.root) or one file; output directory.
INPUT_SOURCE="${1:-$SCRIPT_DIR/../RecoEnergyStudies/output_root/2018_production}"
OUTPUT_DIR="${2:-$SCRIPT_DIR/outputs_2018}"
PROGRAM="$SCRIPT_DIR/build/analysis_RecoEnergyStudies_outputs"
(($# <= 2)) || { echo "Usage: $0 [INPUT_DIRECTORY_OR_FILE [OUTPUT_DIRECTORY]]" >&2; exit 2; }
export LC_ALL=C
shopt -s nullglob
INPUT_FILES=()
if [[ -d "$INPUT_SOURCE" ]]; then
    for file in "$INPUT_SOURCE"/*.root; do
        [[ -f "$file" ]] && INPUT_FILES+=("$file")
    done
elif [[ -f "$INPUT_SOURCE" ]]; then
    INPUT_FILES=("$INPUT_SOURCE")
else
    echo "Missing input directory or file: $INPUT_SOURCE" >&2
    exit 1
fi
((${#INPUT_FILES[@]} > 0)) || { echo "No .root files in $INPUT_SOURCE" >&2; exit 1; }

# Rebuild when the MakeClass header or either source changes.
NEEDS_BUILD=0
[[ -x "$PROGRAM" ]] || NEEDS_BUILD=1
for source in analysis_RecoEnergyStudies_outputs.h analysis_RecoEnergyStudies_outputs.cc main_analysis_RecoEnergyStudies_outputs.cc build_analysis_RecoEnergyStudies_outputs.sh; do
    [[ "$SCRIPT_DIR/$source" -nt "$PROGRAM" ]] && NEEDS_BUILD=1
done
if ((NEEDS_BUILD)); then
    "$SCRIPT_DIR/build_analysis_RecoEnergyStudies_outputs.sh"
fi
mkdir -p -- "$OUTPUT_DIR"
# One process, one TChain, one combined set of histograms/PDF/CSV.
"$PROGRAM" "${INPUT_FILES[@]}" "$OUTPUT_DIR" 2>&1 | tee "$OUTPUT_DIR/analysis.log"
