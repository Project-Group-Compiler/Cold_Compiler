#!/bin/bash

EXECUTABLE="bin/compiler"
TEST_DIR="tests/test_suite"
OUTPUT_DIR="outputs"

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
mkdir -p "$OUTPUT_DIR" 

# Run the executable over each test case
for test_case in "$TEST_DIR"/*.cold; do
  test_name=$(basename "$test_case" .cold)
  echo "Running test case: $test_name"

  # Run the compiler
  "$EXECUTABLE" "$test_case" "--output" "$OUTPUT_DIR/" "$@"

  if [ $? -ne 0 ]; then
    echo "Test $test_name failed during compilation."
    echo
    continue
  fi

  # Check if assembly file was generated
  if [ ! -f "$OUTPUT_DIR/${test_name}.asm" ]; then
    echo "Error: Assembly file '$OUTPUT_DIR/${test_name}.asm' not found after compilation."
    echo "Test $test_name failed."
    echo
    continue
  fi

  # Assemble with nasm
  nasm -f elf32 "$OUTPUT_DIR/${test_name}.asm" -o "$OUTPUT_DIR/${test_name}.o"
  
  if [ $? -ne 0 ]; then
    echo "Test $test_name failed during assembly."
    echo
    continue
  fi

  # Link with gcc
  gcc -m32 -no-pie -z noexecstack "$OUTPUT_DIR/${test_name}.o" -o "$OUTPUT_DIR/exec_${test_name}" -lm

  # "$OUTPUT_DIR/exec_${test_name}"

  if [ $? -ne 0 ]; then
    echo "Test $test_name failed during linking."
    echo
    continue
  fi

  echo "Test $test_name passed successfully."
  echo
done
