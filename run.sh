#!/bin/bash

EXECUTABLE="bin/ir_gen"
TEST_DIR="tests/ir_gen"
OUTPUT_DIR="outputs"

# Ensure test directory exists
if [ ! -d "$TEST_DIR" ]; then
  echo "Error: Test directory '$TEST_DIR' does not exist."
  exit 1
fi

# Create the outputs directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Run the executable over each test case
for test_case in "$TEST_DIR"/*.cold; do
  if [ -f "$test_case" ]; then

    test_name=$(basename "$test_case" .cold)
    echo "Running test case: $test_name"

    "$EXECUTABLE" "$test_case" "--output" "$OUTPUT_DIR/" $@

    if [ $? -eq 0 ]; then
      echo "Test $test_name passed. TAC saved in $OUTPUT_DIR/."
    else
      echo "Test $test_name failed."
    fi
  else
    echo "No test cases found in '$TEST_DIR'."
  fi
done
