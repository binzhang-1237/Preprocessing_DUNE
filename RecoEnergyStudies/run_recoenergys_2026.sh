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
OUTPUT_FILE="${3:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS_2026.root}"


exec "$SCRIPT_DIR/run_recoenergys.sh" "$INPUT_FILE" "$EVENTS" "$OUTPUT_FILE"
