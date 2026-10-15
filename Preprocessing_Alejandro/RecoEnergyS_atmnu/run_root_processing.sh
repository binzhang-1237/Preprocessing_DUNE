#!/bin/bash
#httokensh -a htvaultprod.fnal.gov -i dune -- /bin/bash

pnfsToXRootD(){ printf '%s\n' "${1/\/pnfs\/dune/root:\/\/fndcadoor.fnal.gov:1094\/pnfs\/fnal.gov\/usr\/dune}"; }

# This script automates running a ROOT macro on multiple files.
ROOT_MACRO="make_text_file_to_root_trks_shws.C"

## --- atmospheric ---
#INPUT_DIR="/pnfs/dune/persistent/physicsgroups/duneatmexot/LWanAYankelevichSamples/RecoEnergyStudies/"
#INPUT_DIR="/pnfs/dune/scratch/users/ayankele/recoenergystudies_files"
#OUTPUT_DIR="/pnfs/dune/persistent/users/lwan/nnbar/CVN/atm"
#find "$INPUT_DIR" -type f | while IFS= read -r input_file
#do
#	# Check that it is a regular file before processing
#	echo "$input_file"
#	if [[ -f "$input_file" && -s "$input_file" ]]; then
#		# Get the base name of the file (e.g., "data_01.txt")
#		base_name=$(basename "$input_file")
#		# Remove the file extension to create the output name (e.g., "data_01")
#		output_name="${base_name%.*}"
#		# Define the full path for the output .root file
#		output_file="$OUTPUT_DIR/${output_name}.root"
#
#		if [[ -f "$output_file" && -s "$output_file" ]]; then
#			# If it exists, print a message and skip to the next file
#			echo "⏩ Skipping '$base_name' (output already exists)."
#		else
#			# If it does not exist, process it
#			echo "Processing '$base_name'  ->  '${output_name}.root'"
#			xroot_file="$(pnfsToXRootD "$input_file")"
#			root -l -b -q "${ROOT_MACRO}(\"$xroot_file\", \"$output_file\", \"nnbar\")"
#		fi
#
#	fi
#done


## --- nnbar ---
#INPUT_BASE="/pnfs/dune/persistent/users/lwan/nnbar/preprocess"
#OUTPUT_BASE="/pnfs/dune/persistent/users/lwan/nnbar/CVN"
#
#
#TAG_VAR=(
##  "ha_lfg"
##  "hn_lfg"
##  "ha_esf"
##  "hn_esf"
#  "ha_br"
##  "hn_br"
##   "atm"
#)
#
#for tag in "${TAG_VAR[@]}"; do
#	INPUT_DIR="$INPUT_BASE/$tag"
#	OUTPUT_DIR="$OUTPUT_BASE/$tag"
#	
#	# Create the output directory if it doesn't already exist
#	mkdir -p "$OUTPUT_DIR"
#
#	echo "Input Directory: $INPUT_DIR"
#	echo "Output Directory: $OUTPUT_DIR"
#	echo "-------------------------------------"
#
#	# Loop through every file in the input directory
#	find "$INPUT_DIR" -type f | while IFS= read -r input_file
#	do
#		# Check that it is a regular file before processing
#		echo "$input_file"
#		if [[ -f "$input_file" && -s "$input_file" ]]; then
#			# Get the base name of the file (e.g., "data_01.txt")
#			base_name=$(basename "$input_file")
#			# Remove the file extension to create the output name (e.g., "data_01")
#			output_name="${base_name%.*}"
#			# Define the full path for the output .root file
#			output_file="$OUTPUT_DIR/${output_name}.root"
#	
#			if [[ -f "$output_file" ]] && (( $(wc -c <"$output_file") > 10240 )); then
##			if [[ -f "$output_file" && -s "$output_file" ]]; then
#				# If it exists, print a message and skip to the next file
#				echo "⏩ Skipping '$base_name' (output already exists)."
#			else
#				# If it does not exist, process it
#				echo "Processing '$base_name'  ->  '${output_name}.root'"
#				xroot_file="$(pnfsToXRootD "$input_file")"
#				root -l -b -q "${ROOT_MACRO}(\"$xroot_file\", \"$output_file\", \"nnbar\")"
#			fi
#	
#		fi
#	done
#	
#	echo "-------------------------------------"
#done

# --- wiremod ---
INPUT_BASE="/pnfs/dune/persistent/users/lwan/nnbar/preprocess"
OUTPUT_BASE="/pnfs/dune/persistent/users/lwan/nnbar/CVN"

DETVAR=(
	"ha_br_CV"
)

for tag in "${DETVAR[@]}"; do
	INPUT_DIR="$INPUT_BASE/wiremod/$DETVAR"
	OUTPUT_DIR="$OUTPUT_BASE/wiremod/$DETVAR"
	
	# Create the output directory if it doesn't already exist
	mkdir -p "$OUTPUT_DIR"

	echo "Input Directory: $INPUT_DIR"
	echo "Output Directory: $OUTPUT_DIR"
	echo "-------------------------------------"

	# Loop through every file in the input directory
	find "$INPUT_DIR" -type f | while IFS= read -r input_file
	do
		# Check that it is a regular file before processing
		echo "$input_file"
		if [[ -f "$input_file" && -s "$input_file" ]]; then
			# Get the base name of the file (e.g., "data_01.txt")
			base_name=$(basename "$input_file")
			# Remove the file extension to create the output name (e.g., "data_01")
			output_name="${base_name%.*}"
			# Define the full path for the output .root file
			output_file="$OUTPUT_DIR/${output_name}.root"
	
			if [[ -f "$output_file" ]] && (( $(wc -c <"$output_file") > 10240 )); then
#			if [[ -f "$output_file" && -s "$output_file" ]]; then
				# If it exists, print a message and skip to the next file
				echo "⏩ Skipping '$base_name' (output already exists)."
			else
				# If it does not exist, process it
				echo "Processing '$base_name'  ->  '${output_name}.root'"
				xroot_file="$(pnfsToXRootD "$input_file")"
				root -l -b -q "${ROOT_MACRO}(\"$xroot_file\", \"$output_file\", \"nnbar\")"
			fi
	
		fi
	done
	
	echo "-------------------------------------"
done

echo "✅ All files processed into .root format."
