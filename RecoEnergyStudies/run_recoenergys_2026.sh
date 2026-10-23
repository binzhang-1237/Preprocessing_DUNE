#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Reuse the existing build procedure with the 2026 LArSoft release.
export LARSOFT_VERSION="v10_20_09"
export WORK="$HOME/disk_binzhang/larsoft_v10_20_09"
export DUNE_RELEASE_VERSION="v10_20_09d01"
export FCL_FILE="/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/recoenergys_2026.fcl"
INPUT_FILE="${1:-/mnt/ironwolf_14t2/users/jiaxi/dune2026mc/hd_nue/nue_dune10kt_1x2x6_1063_129_20230823T130817Z_gen_g4_detsim_hitreco_20260713T164258Z_reco2.root}"
EVENTS="${2:-100}"
# The third argument selects a directory; the input determines the filename.
OUTPUT_DIR="${3:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/output_root/2026_production}"
INPUT_NAME="$(basename -- "$INPUT_FILE")"
OUTPUT_NAME="${INPUT_NAME%.root}_RecoEnergyStudies.root"
mkdir -p -- "$OUTPUT_DIR"
# The shared runner changes its working directory, so pass an absolute path.
OUTPUT_DIR="$(cd -- "$OUTPUT_DIR" && pwd)"
OUTPUT_FILE="$OUTPUT_DIR/$OUTPUT_NAME"


exec "$SCRIPT_DIR/run_recoenergys.sh" "$INPUT_FILE" "$EVENTS" "$OUTPUT_FILE"
