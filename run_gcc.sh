#!/bin/bash

# Check if the input file is provided
if [ $# -lt 1 ]; then
    echo "Error: No input file specified!"
    exit 1
fi

# Extract filename without extension for output name
input_file="$1"
filename=$(basename -- "$input_file")
filename_noext="${filename%.*}"

# Step 1: Compile with GCC
echo "Compiling with GCC (Suppressing warnings)..."
gcc -Wno-implicit-function-declaration -Wno-builtin-declaration-mismatch "$input_file" -o "gcc_exec_$filename_noext"

if [ $? -ne 0 ]; then
    echo "GCC compilation failed. Exiting."
    exit 1
fi

# Step 2: Execute GCC compiled program
echo "Running GCC compiled executable..."
./gcc_exec_"$filename_noext"
if [ $? -ne 0 ]; then
    echo "GCC compiled executable crashed!"
    exit 1
fi

# Optionally clean up GCC executable if you want:
# rm gcc_exec_"$filename_noext"
