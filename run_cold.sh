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

# Clean previous builds
make files_clean

# Build your custom compiler
make

# Check if the compiler exists
if [ ! -f "bin/compiler" ]; then
    echo "Error: Compiler not found in bin/compiler."
    exit 1
fi

# Step 1: Run the Cold compiler to generate assembly
bin/compiler $@

# Exit after printing the help message
if [ $? -eq 42 ]; then
    exit 0
fi

# Check if compilation was successful
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
nasm -g -f elf32 "$filename_noext.asm" -o "$filename_noext.o"
if [ $? -ne 0 ]; then
    echo "Assembling failed. Exiting."
    exit 1
fi

# Step 3: Link with gcc to create executable
gcc -m32 -no-pie -z noexecstack "$filename_noext.o" -o "exec_$filename_noext" -lm
if [ $? -ne 0 ]; then
    echo "Linking failed. Exiting."
    exit 1
fi

# Step 4: Run your compiled program with custom compiler's executable
echo "Running custom compiler's executable..."
./exec_"$filename_noext"
if [ $? -ne 0 ]; then
    echo "Custom compiler's executable crashed!"
    exit 1
fi

# Optionally clean up temporary files
# rm exec_"$filename_noext"
