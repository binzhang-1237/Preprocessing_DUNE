#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Use run_recoenergys.sh defaults for the release, workspace and FHiCL.
INPUT_FILE="${1:-/mnt/ironwolf_14t2/users/jiaxi/dune2018mc/nue_dune10kt_1x2x6_13024073_0_20181109T155338_gen_g4_detsim_reco.root}"
EVENTS="${2:-100}"
# The third argument selects a directory; the input determines the filename.
OUTPUT_DIR="${3:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/output_root/2018_production}"
INPUT_NAME="$(basename -- "$INPUT_FILE")"
OUTPUT_NAME="${INPUT_NAME%.root}_RecoEnergyStudies.root"
mkdir -p -- "$OUTPUT_DIR"
# The shared runner changes its working directory, so pass an absolute path.
OUTPUT_DIR="$(cd -- "$OUTPUT_DIR" && pwd)"
OUTPUT_FILE="$OUTPUT_DIR/$OUTPUT_NAME"

exec "$SCRIPT_DIR/run_recoenergys.sh" "$INPUT_FILE" "$EVENTS" "$OUTPUT_FILE"
