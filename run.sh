#!/bin/bash

# Check if the executable is passed as an argument
if [ $# -lt 1 ]; then
  echo "Usage: ./run.sh <compiler_executable>"
  exit 1
fi

# Compiler executable
EXECUTABLE=$1

# Directory containing test cases
TEST_DIR="tests"
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
    # Get the base name of the test case (without extension)
    test_name=$(basename "$test_case" .cold)

    echo "Running test case: $test_name"

    # Redirect output to the output directory
    "$EXECUTABLE" "$test_case" > "$OUTPUT_DIR/$test_name.output"

    if [ $? -eq 0 ]; then
      echo "Test $test_name passed. Output saved to $OUTPUT_DIR/$test_name.output"
    else
      echo "Test $test_name failed."
    fi
  else
    echo "No test cases found in '$TEST_DIR'."
  fi
done
