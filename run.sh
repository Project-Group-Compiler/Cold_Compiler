#!/bin/bash

EXECUTABLE="bin/parser"
TEST_DIR="tests/parser"
OUTPUT_DIR="outputs"
AST_DIR="ast"

# Ensure test directory exists
if [ ! -d "$TEST_DIR" ]; then
  echo "Error: Test directory '$TEST_DIR' does not exist."
  exit 1
fi

# Create the outputs directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"
mkdir -p "$AST_DIR"

# Run the executable over each test case
for test_case in "$TEST_DIR"/*.cold; do
  if [ -f "$test_case" ]; then

    test_name=$(basename "$test_case" .cold)
    echo "Running test case: $test_name"

    "$EXECUTABLE" "$test_case" "-o" "$OUTPUT_DIR/" "--astdir" "$AST_DIR/" "-a" "-s" "-f"

    if [ $? -eq 0 ]; then
      echo "Test $test_name passed. Output saved in $OUTPUT_DIR/. AST saved in $AST_DIR/."
    else
      echo "Test $test_name failed."
    fi

    dot -Tpng "$AST_DIR/$test_name""_AST.dot" -o "$AST_DIR/$test_name""_ast.png"
    rm "$AST_DIR/$test_name""_AST.dot"

  else
    echo "No test cases found in '$TEST_DIR'."
  fi
done
