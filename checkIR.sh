#!/bin/bash

# Directories and executable
IR_DIR="outputs/IR"
IR_CORR_DIR="outputs/IR_newCorr"

# Ensure output and reference IR directories exist
if [ ! -d "$IR_DIR" ]; then
  echo "Error: IR directory '$IR_DIR' does not exist."
  exit 1
fi

if [ ! -d "$IR_CORR_DIR" ]; then
  echo "Error: Reference IR directory '$IR_CORR_DIR' does not exist."
  exit 1
fi

# Compare each generated IR to the corresponding correct IR
for ir_file in "$IR_DIR"/*.txt; do
  if [ -f "$ir_file" ]; then
    test_name=$(basename "$ir_file")
    ref_file="$IR_CORR_DIR/$test_name"

    if [ ! -f "$ref_file" ]; then
      echo "Missing reference IR for $test_name"
      continue
    fi

    if diff -q "$ir_file" "$ref_file" > /dev/null; then
      echo "[PASS] $test_name"
    else
      echo "[FAIL] $test_name - Output differs from reference"
      # Optional: Uncomment the line below to see the difference
      diff "$ir_file" "$ref_file"
    fi
  fi
done
