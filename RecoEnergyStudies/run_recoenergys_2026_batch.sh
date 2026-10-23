#!/usr/bin/env bash

set -eo pipefail

# User settings: one job = one input file; jobs run strictly one at a time.
JOB_COUNT=20             # 0 = all files; a positive integer = the first N files.
EVENTS_PER_FILE=100     # -1 = all events in each input file.
INPUT_DIR="/mnt/ironwolf_14t2/users/jiaxi/dune2026mc/hd_nue"
LOG_DIR="/home/binzhang/disk_binzhang/preprocessing_multi_production/RecoEnergyStudies/log/2026_production"


SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
RUNNER="$SCRIPT_DIR/run_recoenergys_2026.sh"


[[ "$JOB_COUNT" =~ ^(0|[1-9][0-9]*)$ ]] || { echo "Invalid JOB_COUNT: $JOB_COUNT" >&2; exit 2; }
[[ "$EVENTS_PER_FILE" =~ ^(-1|0|[1-9][0-9]*)$ ]] || { echo "Invalid EVENTS_PER_FILE: $EVENTS_PER_FILE" >&2; exit 2; }
[[ -d "$INPUT_DIR" ]] || { echo "Missing input directory: $INPUT_DIR" >&2; exit 1; }
[[ -x "$RUNNER" ]] || { echo "Missing executable runner: $RUNNER" >&2; exit 1; }

# Bash glob gives a stable filename order. Only direct children are included.
export LC_ALL=C
shopt -s nullglob
FILES=("$INPUT_DIR"/*.root)
((${#FILES[@]} > 0)) || { echo "No .root files found in $INPUT_DIR" >&2; exit 1; }
TOTAL=${#FILES[@]}
if ((JOB_COUNT > 0 && JOB_COUNT < TOTAL)); then
    TOTAL=$JOB_COUNT
fi

mkdir -p -- "$LOG_DIR"
LOG_DIR="$(cd -- "$LOG_DIR" && pwd)"
SUMMARY="$LOG_DIR/summary.tsv"
printf 'status\texit_code\tinput_file\tlog_file\n' > "$SUMMARY"
printf 'Files to process: %d / %d\nEvents per file: %s\nLogs: %s\n' "$TOTAL" "${#FILES[@]}" "$EVENTS_PER_FILE" "$LOG_DIR"

SUCCEEDED=0
FAILED=0
for ((index=0; index<TOTAL; ++index)); do
    input_file="${FILES[index]}"
    input_name="$(basename -- "$input_file")"
    log_file="$LOG_DIR/${input_name%.root}.log"
    printf '\n[%d/%d] Starting: %s\n' "$((index+1))" "$TOTAL" "$input_name"

    # Foreground execution: wait for this file to finish before starting the next.
    # No third argument: use run_recoenergys_2026.sh's default output directory.
    if "$RUNNER" "$input_file" "$EVENTS_PER_FILE" > "$log_file" 2>&1; then
        SUCCEEDED=$((SUCCEEDED+1))
        printf 'OK\t0\t%s\t%s\n' "$input_file" "$log_file" >> "$SUMMARY"
        printf '[%d/%d] Finished: %s\n' "$((index+1))" "$TOTAL" "$input_name"
    else
        status=$?
        FAILED=$((FAILED+1))
        printf 'FAILED\t%d\t%s\t%s\n' "$status" "$input_file" "$log_file" >> "$SUMMARY"
        printf '[%d/%d] FAILED (exit %d): %s\nLog: %s\n' "$((index+1))" "$TOTAL" "$status" "$input_name" "$log_file" >&2
        tail -n 12 "$log_file" >&2
        # Interrupts stop the batch; ordinary job failures allow later files to run.
        if ((status == 130 || status == 143)); then
            exit "$status"
        fi
    fi
done

printf '\nBatch finished: %d succeeded, %d failed.\nSummary: %s\n' "$SUCCEEDED" "$FAILED" "$SUMMARY"
((FAILED == 0))
