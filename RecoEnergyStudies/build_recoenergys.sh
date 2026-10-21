#!/usr/bin/env bash

set -eo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
WORK="${LARSOFT_WORK:-$HOME/disk_binzhang/larsoft_v10_02_02}"
LARSOFT_VERSION="${LARSOFT_VERSION:-v10_02_02}"
ROOT_LOCAL="${ROOT_LOCAL:-$HOME/root-local-6.28.12}"
BUILD_DIR="${BUILD_DIR:-$WORK/build_recoenergys_slf7.x86_64}"
JOBS="${JOBS:-$(nproc)}"
LOCAL_PRODUCTS="$WORK/localProducts_larsoft_${LARSOFT_VERSION}_e26_prof"
CLEAN=1

usage() {
    cat <<'EOF'
Usage: ./build_recoenergys.sh [-j N] [--no-clean]

Build the RecoEnergyS_module plugin using the local LArSoft/DUNE environment.

Options:
  -j N         Use N parallel build jobs. Default: all CPUs.
  --no-clean   Keep the existing CMake cache and reconfigure incrementally.
  -h, --help   Show this help.

Environment overrides:
  LARSOFT_WORK, LARSOFT_VERSION, ROOT_LOCAL, BUILD_DIR, JOBS
EOF
}

source_required() {
    local description="$1"
    local setup_file="$2"
    local status

    printf '[build] %s\n' "$description"
    if source "$setup_file"; then
        return 0
    else
        status=$?
        printf '[build] ERROR: %s failed (exit %d): %s\n' \
            "$description" "$status" "$setup_file" >&2
        return "$status"
    fi
}

while (($# > 0)); do
    case "$1" in
        -j)
            (($# >= 2)) || { echo "Missing value for -j" >&2; exit 2; }
            JOBS="$2"
            shift 2
            ;;
        --no-clean)
            CLEAN=0
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

[[ "$JOBS" =~ ^[1-9][0-9]*$ ]] || { echo "JOBS must be a positive integer, got '$JOBS'" >&2; exit 2; }
[[ -f "$SCRIPT_DIR/RecoEnergyS_module.cc" ]] || { echo "Missing $SCRIPT_DIR/RecoEnergyS_module.cc" >&2; exit 1; }
[[ -f "$SCRIPT_DIR/CMakeLists.txt" ]] || { echo "Missing $SCRIPT_DIR/CMakeLists.txt" >&2; exit 1; }
[[ -f "$SCRIPT_DIR/standalone/CMakeLists.txt" ]] || { echo "Missing $SCRIPT_DIR/standalone/CMakeLists.txt" >&2; exit 1; }
[[ -f "$WORK/setup_larsoft.sh" ]] || { echo "Missing $WORK/setup_larsoft.sh" >&2; exit 1; }
[[ -f "$WORK/setup_dune.sh" ]] || { echo "Missing $WORK/setup_dune.sh" >&2; exit 1; }
[[ -d "$ROOT_LOCAL" ]] || { echo "Missing ROOT installation: $ROOT_LOCAL" >&2; exit 1; }
[[ -f "$LOCAL_PRODUCTS/setup" ]] || { echo "Missing localProducts setup: $LOCAL_PRODUCTS/setup" >&2; exit 1; }
[[ -f "$ROOT_LOCAL/bin/thisroot.sh" ]] || { echo "Missing ROOT setup: $ROOT_LOCAL/bin/thisroot.sh" >&2; exit 1; }
[[ -f "$WORK/srcs/dunereco/dunereco/FDSensOpt/FDSensOptData/EnergyRecoOutput.h" ]] || {
    echo "Missing DUNE header under $WORK/srcs/dunereco/dunereco" >&2
    exit 1
}
[[ -d "$WORK/build_slf7.x86_64/dunereco" ]] || {
    echo "Missing DUNE build products under $WORK/build_slf7.x86_64/dunereco" >&2
    exit 1
}

cd "$WORK"
export LARSOFT_WORK="$WORK"
export prod_db=/cvmfs/larsoft.opensciencegrid.org/products
source_required "Initializing LArSoft environment" "$WORK/setup_larsoft.sh"
source_required "Initializing DUNE environment" "$WORK/setup_dune.sh"
printf '[build] Selecting LArSoft %s with qualifier e26:prof\n' "$LARSOFT_VERSION"
if setup larsoft "$LARSOFT_VERSION" -q e26:prof; then
    :
else
    status=$?
    printf '[build] ERROR: selecting LArSoft %s failed (exit %d)\n' \
        "$LARSOFT_VERSION" "$status" >&2
    exit "$status"
fi
source_required "Loading local products" "$LOCAL_PRODUCTS/setup"
export PRODUCTS="/cvmfs/dune.opensciencegrid.org/products/dune:${PRODUCTS:-}"
export PATH="/mnt/ssd/larsoft/mrb_patch/bin:$PATH"
printf '[build] Configuring MRB build environment\n'
if mrbsetenv; then
    :
else
    status=$?
    printf '[build] ERROR: configuring MRB environment failed (exit %d)\n' \
        "$status" >&2
    exit "$status"
fi

if [[ "${MRB_PROJECT_VERSION:-}" != "$LARSOFT_VERSION" ]]; then
    echo "Loaded MRB version '${MRB_PROJECT_VERSION:-unknown}', expected '$LARSOFT_VERSION'" >&2
    exit 1
fi

source_required "Loading ROOT environment" "$ROOT_LOCAL/bin/thisroot.sh"

# ROOT 6.28.12 was built for Python 3.10; preserve UPS's compiler environment.
PYTHON_SHIM="$WORK/.python-bin"
mkdir -p "$PYTHON_SHIM"
ln -sfn /usr/bin/python3 "$PYTHON_SHIM/python"
export PATH="$PYTHON_SHIM:$PATH"
unset PYTHONHOME

export LD_LIBRARY_PATH="$HOME/local/usr/lib/x86_64-linux-gnu:$HOME/local/jpeg/lib:/usr/lib/x86_64-linux-gnu:$ROOT_LOCAL/lib:${LD_LIBRARY_PATH:-}"
export LIBRARY_PATH="$HOME/local/jpeg/lib:/usr/lib/x86_64-linux-gnu:$ROOT_LOCAL/lib:${LIBRARY_PATH:-}"
unset CPATH C_INCLUDE_PATH CPLUS_INCLUDE_PATH OBJC_INCLUDE_PATH ROOT_INCLUDE_PATH
export CMAKE_PREFIX_PATH="$ROOT_LOCAL:$HOME/local/jpeg:${CMAKE_PREFIX_PATH:-}"
export CXXFLAGS="-isystem /usr/include/x86_64-linux-gnu${CXXFLAGS:+ $CXXFLAGS}"
export CFLAGS="-isystem /usr/include/x86_64-linux-gnu${CFLAGS:+ $CFLAGS}"
export LDFLAGS="-fuse-ld=gold${LDFLAGS:+ $LDFLAGS}"
export CLING_STANDALONE=1

command -v cmake >/dev/null || { echo "cmake was not found after environment setup" >&2; exit 1; }
command -v root-config >/dev/null || { echo "root-config was not found after environment setup" >&2; exit 1; }
[[ -n "${GEANT4_FQ_DIR:-}" ]] || { echo "GEANT4_FQ_DIR was not set by the LArSoft environment" >&2; exit 1; }
GEANT4_CONFIG="$(find "$GEANT4_FQ_DIR" -name Geant4Config.cmake -print -quit)"
[[ -n "$GEANT4_CONFIG" ]] || { echo "Geant4Config.cmake not found under $GEANT4_FQ_DIR" >&2; exit 1; }

CMAKE_SOURCE_DIR="$SCRIPT_DIR/standalone"
if [[ -f "$BUILD_DIR/CMakeCache.txt" ]]; then
    existing_source="$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$BUILD_DIR/CMakeCache.txt")"
    if [[ "$existing_source" != "$CMAKE_SOURCE_DIR" ]]; then
        if ((CLEAN)); then
            rm -rf "$BUILD_DIR/CMakeCache.txt" "$BUILD_DIR/CMakeFiles"
        else
            echo "Build directory uses a different CMake source: $existing_source" >&2
            echo "Run without --no-clean once to configure from $CMAKE_SOURCE_DIR" >&2
            exit 1
        fi
    fi
fi
if ((CLEAN)); then
    rm -rf "$BUILD_DIR/CMakeCache.txt" "$BUILD_DIR/CMakeFiles"
fi

mkdir -p "$BUILD_DIR"
ln -sfn "$WORK/srcs/dunereco/dunereco" "$BUILD_DIR/dune"

printf '[build] Configuration summary:\nLArSoft=%s\nROOT=%s\nPython=%s\nSource=%s\nBuild=%s\nJobs=%s\n' \
    "$MRB_PROJECT_VERSION" "$(root-config --version)" "$(python --version 2>&1)" \
    "$SCRIPT_DIR" "$BUILD_DIR" "$JOBS"

printf '[build] Configuring CMake\n'
cmake -S "$CMAKE_SOURCE_DIR" -B "$BUILD_DIR" \
    -DRECOENERGY_STUDIES_SOURCE="$SCRIPT_DIR" \
    -DLARSOFT_DUNE_SOURCE_DIR="$WORK/srcs/dunereco/dunereco" \
    -DLARSOFT_DUNE_BUILD_DIR="$WORK/build_slf7.x86_64/dunereco" \
    -DGeant4_DIR="$(dirname "$GEANT4_CONFIG")"
printf '[build] Building RecoEnergyS_module\n'
cmake --build "$BUILD_DIR" --target RecoEnergyS_module SignalShapingServiceDUNE_service --parallel "$JOBS"
