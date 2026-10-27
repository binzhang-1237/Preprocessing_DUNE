#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

INPUT_FILE="${1:-/mnt/ironwolf_14t2/users/jiaxi/dune2026mc/hd_nue/nue_dune10kt_1x2x6_1063_129_20230823T130817Z_gen_g4_detsim_hitreco_20260713T164258Z_reco2.root}"
EVENTS="${2:-100}"
OUTPUT_DIR="${3:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/output_root/2026_production/test}"
INPUT_NAME="$(basename -- "$INPUT_FILE")"
mkdir -p -- "$OUTPUT_DIR"

clear
"$SCRIPT_DIR/build_recoenergys_2026.sh"
exec "$SCRIPT_DIR/run_recoenergys_2026.sh" "$INPUT_FILE" "$EVENTS" "$OUTPUT_DIR"
