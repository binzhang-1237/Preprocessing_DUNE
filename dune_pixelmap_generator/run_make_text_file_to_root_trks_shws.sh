#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
MACRO="$SCRIPT_DIR/macros/make_text_file_to_root_trks_shws.C"
INPUT_FILE="${1:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/output_root/2026_production/test/nue_dune10kt_1x2x6_1063_129_20230823T130817Z_gen_g4_detsim_hitreco_20260713T164258Z_reco2_RecoEnergyStudies.root}"
OUTPUT_FILE="${2:-/home/binzhang/disk_binzhang/preprocessing_multi_production/dune_pixelmap_generator/output/pixel_maps_test.root}"
TYPE="${3:-numu}"
ROOT_LOCAL="${ROOT_LOCAL:-$HOME/root-local-6.28.12}"

[[ -f "$INPUT_FILE" ]] || { echo "Missing input: $INPUT_FILE" >&2; exit 1; }
[[ -f "$MACRO" ]] || { echo "Missing macro: $MACRO" >&2; exit 1; }
[[ "$TYPE" == "nue" || "$TYPE" == "numu" ]] || {
    echo "Type must be nue or numu" >&2
    exit 2
}
[[ ! -e "$OUTPUT_FILE" || ! "$INPUT_FILE" -ef "$OUTPUT_FILE" ]] || {
    echo "Input and output must be different files" >&2
    exit 2
}

# Load ROOT only; no LArSoft, UPS or DUNE setup is needed.
source "$ROOT_LOCAL/bin/thisroot.sh"
export LD_LIBRARY_PATH="$HOME/local/usr/lib/x86_64-linux-gnu:$HOME/local/jpeg/lib:${LD_LIBRARY_PATH:-}"
command -v root >/dev/null || { echo "root was not found" >&2; exit 1; }
mkdir -p -- "$(dirname -- "$OUTPUT_FILE")"

# Escape C++ string arguments without eval or shell re-parsing.
cpp_quote() {
    local value="$1"
    value="${value//\\/\\\\}"
    value="${value//\"/\\\"}"
    printf '"%s"' "$value"
}

printf 'Input=%s\nOutput=%s\nType=%s\n' "$INPUT_FILE" "$OUTPUT_FILE" "$TYPE"
root -l -b -q "$MACRO($(cpp_quote "$INPUT_FILE"),$(cpp_quote "$OUTPUT_FILE"),$(cpp_quote "$TYPE"))"

# Convert the pixelmap tree using the isolated Python environment.
HDF5_FILE="${OUTPUT_FILE%.*}.h5"
printf 'HDF5=%s\n' "$HDF5_FILE"
exec "$HOME/local/root2hdf5/bin/root2hdf5" -i "$OUTPUT_FILE" -o "$HDF5_FILE" -t pixelmap
