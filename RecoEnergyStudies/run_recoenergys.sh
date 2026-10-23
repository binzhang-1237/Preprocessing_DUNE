#!/usr/bin/env bash

set -eo pipefail

# Reproducible local runtime for the 2018-production TransformerCVN job.
WORK="${WORK:-$HOME/disk_binzhang/larsoft_v10_02_02}"
LARSOFT_VERSION="${LARSOFT_VERSION:-v10_02_02}"
DUNE_RELEASE_VERSION="${DUNE_RELEASE_VERSION:-v10_02_02d00}"
ROOT_LOCAL="$HOME/root-local-6.28.12"
ROOT_RUNTIME="$ROOT_LOCAL"
BUILD_DIR="$WORK/build_recoenergys_slf7.x86_64"
LOCAL_PRODUCTS="$WORK/localProducts_larsoft_${LARSOFT_VERSION}_e26_prof"
FCL_FILE="${FCL_FILE:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/recoenergys_2018.fcl}"
INPUT_FILE="${1:-/mnt/ironwolf_14t2/users/jiaxi/dune2018mc/nue_dune10kt_1x2x6_13024073_0_20181109T155338_gen_g4_detsim_reco.root}"
EVENTS="${2:-100}"
OUTPUT_FILE="${3:-/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/RecoEnergyS.root}"
LOG_FILE="/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/run_recoenergys.log"

[[ -f "$WORK/setup_dune.sh" ]] || { echo "Missing $WORK/setup_dune.sh" >&2; exit 1; }
[[ -f "$LOCAL_PRODUCTS/setup" ]] || { echo "Missing $LOCAL_PRODUCTS/setup" >&2; exit 1; }
[[ -f "$ROOT_LOCAL/bin/thisroot.sh" ]] || { echo "Missing ROOT setup: $ROOT_LOCAL" >&2; exit 1; }
[[ -f "$ROOT_RUNTIME/bin/thisroot.sh" ]] || { echo "Missing runtime ROOT setup: $ROOT_RUNTIME" >&2; exit 1; }
[[ -f "$FCL_FILE" ]] || { echo "Missing FHiCL file: $FCL_FILE" >&2; exit 1; }
[[ -f "$INPUT_FILE" ]] || { echo "Missing input ROOT file: $INPUT_FILE" >&2; exit 1; }
[[ -f "$BUILD_DIR/lib/libRecoEnergyS_module.so" ]] || {
    echo "Missing RecoEnergyS module library: $BUILD_DIR/lib/libRecoEnergyS_module.so; run build_recoenergys.sh first" >&2
    exit 1
}

cd "$WORK"
export LARSOFT_WORK="$WORK"
export prod_db=/cvmfs/larsoft.opensciencegrid.org/products
source setup_larsoft.sh || true
source setup_dune.sh || true
setup larsoft "$LARSOFT_VERSION" -q e26:prof || true
source "$LOCAL_PRODUCTS/setup" || true
export PRODUCTS="/cvmfs/dune.opensciencegrid.org/products/dune:${PRODUCTS:-}"
export PATH="/mnt/ssd/larsoft/mrb_patch/bin:$PATH"
mrbsetenv || true

if [[ "${MRB_PROJECT_VERSION:-}" != "$LARSOFT_VERSION" ]]; then
    echo "Loaded MRB version '${MRB_PROJECT_VERSION:-unknown}', expected '$LARSOFT_VERSION'" >&2
    exit 1
fi

if setup dunesim "$DUNE_RELEASE_VERSION" -q e26:prof; then
    :
else
    status=$?
    echo "Failed to set up dunesim $DUNE_RELEASE_VERSION (exit $status)" >&2
    exit "$status"
fi

if setup duneopdet "$DUNE_RELEASE_VERSION" -q e26:prof; then
    :
else
    status=$?
    echo "Failed to set up duneopdet $DUNE_RELEASE_VERSION (exit $status)" >&2
    exit "$status"
fi

if setup duneprototypes "$DUNE_RELEASE_VERSION" -q e26:prof; then
    :
else
    status=$?
    echo "Failed to set up duneprototypes $DUNE_RELEASE_VERSION (exit $status)" >&2
    exit "$status"
fi

if setup dunedataprep "$DUNE_RELEASE_VERSION" -q e26:prof; then
    :
else
    status=$?
    echo "Failed to set up dunedataprep $DUNE_RELEASE_VERSION (exit $status)" >&2
    exit "$status"
fi

if setup dunesw "$DUNE_RELEASE_VERSION" -q e26:prof; then
    :
else
    status=$?
    echo "Failed to set up dunesw $DUNE_RELEASE_VERSION (exit $status)" >&2
    exit "$status"
fi

source "$ROOT_RUNTIME/bin/thisroot.sh"
unset CPATH C_INCLUDE_PATH CPLUS_INCLUDE_PATH OBJC_INCLUDE_PATH
unset PYTHONHOME

export LD_LIBRARY_PATH="$BUILD_DIR/lib:$HOME/local/usr/lib/x86_64-linux-gnu:$HOME/local/jpeg/lib:/usr/lib/x86_64-linux-gnu:$ROOT_LOCAL/lib:${LD_LIBRARY_PATH:-}"
export FHICL_FILE_PATH="$(dirname "$FCL_FILE")${FHICL_FILE_PATH:+:$FHICL_FILE_PATH}"
export CET_PLUGIN_PATH="$BUILD_DIR/lib${CET_PLUGIN_PATH:+:$CET_PLUGIN_PATH}"

command -v lar >/dev/null || { echo "lar was not found after environment setup" >&2; exit 1; }
printf 'LArSoft=%s\nROOT=%s\nlar=%s\nFHiCL=%s\nInput=%s\nEvents=%s\n' \
    "$MRB_PROJECT_VERSION" "$(root-config --version)" "$(command -v lar)" \
    "$FCL_FILE" "$INPUT_FILE" "$EVENTS"


printf 'Starting lar at %s\n' "$(date -Is)" | tee "$LOG_FILE"
stdbuf -oL -eL lar -n "$EVENTS" -c "$FCL_FILE" -s "$INPUT_FILE" 2>&1 | tee -a "$LOG_FILE"
# rm RootOutput*.root
rm $WORK/TFileService*.root || true
mv $WORK/RecoEnergyS.root $OUTPUT_FILE

