#!/bin/bash

# Specify input file as the first argument

# Check if the compiler exists

if [ ! -f "bin/compiler" ]; then
    echo "Error: Compiler not found in bin/compiler."
    exit 1
fi

# if no of args < 1 error
if [ $# -lt 1 ]; then
    echo "Error: No input file specified!"
    exit 1
fi

# Extract filename without extension for output name
input_file="$1"
filename=$(basename -- "$input_file")
filename_noext="${filename%.*}"

# Step 1: Run the Cold compiler to generate assembly
bin/compiler $@

# Exit after printing the help message
if [ $? -eq 42 ]; then
    exit 0
fi

# Check if compiler was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Assembly file should be named the same as input but with .asm extension

if [ ! -f "$filename_noext.asm" ]; then
    echo "Error: Assembly file "$filename_noext.asm" not found after compilation."
    exit 1
fi

# Step 2: Assemble with nasm and link with gcc in a pipeline
nasm -f elf32 "$filename_noext.asm" -o "$filename_noext.o"

if [ $? -ne 0 ]; then
    echo "Assembling failed. Exiting."
    exit 1
fi

# Step 3: Link with gcc
gcc -m32 -no-pie -z noexecstack "$filename_noext.o" -o "exec_$filename_noext" -lm

if [ $? -ne 0 ]; then
    echo "Linking failed. Exiting."
    exit 1
fi
