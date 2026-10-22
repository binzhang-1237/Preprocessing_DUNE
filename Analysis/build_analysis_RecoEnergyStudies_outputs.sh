#!/usr/bin/env bash
set -eo pipefail
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/root_environment.sh"
mkdir -p "$SCRIPT_DIR/build"
cd "$SCRIPT_DIR/build"
# Generate dictionaries so nested numeric vectors are read as real C++ objects.
cat > NumericVectors.h <<'EOF'
#include <vector>
EOF
cat > NumericVectorsLinkDef.h <<'EOF'
#ifdef __CLING__
#pragma link C++ class std::vector<int>+;
#pragma link C++ class std::vector<double>+;
#pragma link C++ class std::vector<std::vector<int>>+;
#pragma link C++ class std::vector<std::vector<double>>+;
#endif
EOF
"$ROOT_LOCAL/bin/rootcling" -f NumericVectorsDict.cc -c NumericVectors.h NumericVectorsLinkDef.h
read -r -a ROOT_CFLAGS <<< "$("$ROOT_LOCAL/bin/root-config" --cflags)"
read -r -a ROOT_LIBS <<< "$("$ROOT_LOCAL/bin/root-config" --libs)"
"${CXX:-g++}" "${ROOT_CFLAGS[@]}" -std=c++17 -O2 -Wall -Wextra \
    "$SCRIPT_DIR/analysis_RecoEnergyStudies_outputs.cc" \
    "$SCRIPT_DIR/main_analysis_RecoEnergyStudies_outputs.cc" NumericVectorsDict.cc \
    "${ROOT_LIBS[@]}" -Wl,-rpath,"$ROOT_LOCAL/lib" \
    -o analysis_RecoEnergyStudies_outputs
printf 'Built: %s/build/analysis_RecoEnergyStudies_outputs\n' "$SCRIPT_DIR"
