#!/bin/bash

# --- Configuration ---
PYTHON_SCRIPT="preprocess_atmnu.py"
OUTPUT_DIR="/exp/dune/data/users/linyan/nnbar/h5/step4/wiremod"
H5_FILENAME="nnbar_ha_br_CV step4.h5"
LOG_FILENAME="${H5_FILENAME}.log"
OUTPUT_H5="$OUTPUT_DIR/$H5_FILENAME"
LOG_FILE="$OUTPUT_DIR/$LOG_FILENAME"

# First directory and the number of files to take from it
DIR1="/pnfs/dune/persistent/users/lwan/nnbar/CVN/atm"
#LIMIT1=10
LIMIT1=0

# Second directory and the number of files to take from it
TAG_VAR=(
#	"ha_lfg"
#	"hn_lfg"
#	"ha_esf"
#	"hn_esf"
#	"ha_br"
#	"hn_br"
	"ha_br_CV"
)
DIR2_BASE="/pnfs/dune/persistent/users/lwan/nnbar/CVN/wiremod/"
#DIR2_BASE="/pnfs/dune/persistent/users/lwan/nnbar/CVN/"
#LIMIT2=3
LIMIT2=1000

# Exit immediately if a command fails
#set -e

mapfile -t files_from_dir1 < <(find "$DIR1" -maxdepth 1 -type f -size +10000c -not -name '.*' -print0 | LC_ALL=C sort -z | xargs -0 -r printf '%s\n'| head -n "$LIMIT1")
#mapfile -t files_from_dir1 < <(find "$DIR1" -maxdepth 1 -type f -size +0c -not -name '.*' | head -n "$LIMIT1")
echo "Found ${#files_from_dir1[@]} files in '$DIR1'."

# Start the master list with DIR1 files (only if non-empty)
all_input_files=()
if ((${#files_from_dir1[@]})); then
	all_input_files+=("${files_from_dir1[@]}")
fi

# For each tag directory, collect and append
for tag in "${TAG_VAR[@]}"; do
	DIR2="$DIR2_BASE/$tag"
	mapfile -t tmp_list < <(find "$DIR2" -maxdepth 1 -type f -size +0c -not -name '.*' -print0 | LC_ALL=C sort -z | xargs -0 -r printf '%s\n' | head -n "$LIMIT2")
#	mapfile -t tmp_list < <(find "$DIR2" -maxdepth 1 -type f -size +0c -not -name '.*' | head -n "$LIMIT2")
	echo "Found ${#tmp_list[@]} files in '$DIR2'."

	if ((${#tmp_list[@]})); then
		all_input_files+=("${tmp_list[@]}")
	fi
done

printf "%s\n" "${all_input_files[@]}" > "$LOG_FILE"
echo "📝 List of all input files saved to '$LOG_FILE'."

# Check if any files were found before proceeding
if [ ${#all_input_files[@]} -eq 0 ]; then
	echo "❌ Error: No input files were found in the specified directories. Exiting."
	exit 1
fi

total_files=${#all_input_files[@]}
echo "-------------------------------------"
echo "Total files to process: $total_files"
echo "Output file: $OUTPUT_H5"
echo "🚀 Running Python script..."

python "$PYTHON_SCRIPT" "$OUTPUT_H5" "$LOG_FILE"

echo "-------------------------------------"

echo "✅ Python script finished."
