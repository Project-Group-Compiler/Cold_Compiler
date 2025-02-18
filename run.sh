#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Error: No test directory provided."
  echo "Usage: $0 <test_directory>"
  exit 1
fi

EXECUTABLE="bin/parser"
TEST_DIR=$1
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

    "$EXECUTABLE" "$test_case" "-o" "$OUTPUT_DIR/" "-a" "-f"

    if [ $? -eq 0 ]; then
      echo "Test $test_name passed. Output saved in $OUTPUT_DIR/."
      dot -Tpng "$OUTPUT_DIR/$test_name""_AST.dot" -o "$OUTPUT_DIR/$test_name""_ast.png"
      rm "$OUTPUT_DIR/$test_name""_AST.dot"
    else
      echo "Test $test_name failed."
    fi
  
  else
    echo "No test cases found in '$TEST_DIR'."
  fi
done