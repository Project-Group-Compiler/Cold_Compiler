#!/bin/bash

EXECUTABLE="bin/ir_gen"
TEST_DIR="tests/ir_gen"
OUTPUT_DIR="outputs/IR"
OUTPUT_DIR_OPT="outputs/opt"

# Ensure test directory exists
if [ ! -d "$TEST_DIR" ]; then
  echo "Error: Test directory '$TEST_DIR' does not exist."
  exit 1
fi

# Ensure executable exists
if [ ! -f "$EXECUTABLE" ]; then
  echo "Error: Executable '$EXECUTABLE' does not exist."
  exit 1
fi

# Create the outputs directory if it doesn't exist
mkdir -p "$OUTPUT_DIR" "$OUTPUT_DIR_OPT"

# Run the executable over each test case
for test_case in "$TEST_DIR"/*.cold; do
  if [ -f "$test_case" ]; then

    test_name=$(basename "$test_case" .cold)
    echo "Running test case: $test_name"

    "$EXECUTABLE" "$test_case" "--output" "$OUTPUT_DIR/" "-O0" "$@"


    if [ $? -eq 0 ]; then
      echo "Test $test_name passed."
      echo
    else
      echo "Test $test_name failed."
      echo
      continue
    fi

    "$EXECUTABLE" "$test_case" "--output" "$OUTPUT_DIR_OPT/" "$@"

  else
    echo "No test cases found in '$TEST_DIR'."
  fi
done