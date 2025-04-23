#!/bin/bash

# Clean previous builds
make files_clean

# Build your compiler
make

if [ ! -f "bin/compiler" ]; then
    echo "Error: Compiler not found in bin/compiler."
    exit 1
fi

# Check if input file is provided
if [ $# -lt 1 ]; then
    echo "Error: No input file specified!"
    exit 1
fi

# Extract filename without extension
input_file="$1"
filename=$(basename -- "$input_file")
filename_noext="${filename%.*}"

# Step 1: Run your compiler to generate assembly
bin/compiler "$@"

# Exit after printing help message
if [ $? -eq 42 ]; then
    exit 0
fi

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Exiting."
    exit 1
fi

# Check if assembly file was created
if [ ! -f "$filename_noext.asm" ]; then
    echo "Error: Assembly file \"$filename_noext.asm\" not found after compilation."
    exit 1
fi

# Step 2: Assemble the .asm file
nasm -g -f elf32 "$filename_noext.asm" -o "$filename_noext.o"

if [ $? -ne 0 ]; then
    echo "Assembling failed. Exiting."
    exit 1
fi

# Step 3: Link the object file
gcc -Wno-implicit-function-declaration -Wno-builtin-declaration-mismatch -m32 -no-pie -z noexecstack "$filename_noext.o" -o "exec_$filename_noext" -lm

if [ $? -ne 0 ]; then
    echo "Linking failed. Exiting."
    exit 1
fi

# Step 4: Compile the original C file with gcc C89
gcc -Wno-implicit-function-declaration -Wno-builtin-declaration-mismatch -m32 -no-pie -z noexecstack "$input_file" -o "gcc_exec_$filename_noext" -lm

if [ $? -ne 0 ]; then
    echo "GCC C89 compilation failed. Exiting."
    exit 1
fi

# Step 5: Run both binaries and capture their outputs
./exec_"$filename_noext" > output_my_compiler.txt
if [ $? -ne 0 ]; then
    echo "Error: Your compiled executable crashed!"
fi

./gcc_exec_"$filename_noext" > output_gcc.txt
if [ $? -ne 0 ]; then
    echo "Error: GCC compiled executable crashed!"
fi

# Step 6: Compare outputs
diff -u output_gcc.txt output_my_compiler.txt
if [ $? -eq 0 ]; then
    echo "✅ Outputs match! Your compiler works correctly on this input."

    # Make directory if it doesn't exist
    mkdir -p tests/matched_outputs

    # Move or copy outputs to matched_outputs
    cp output_my_compiler.txt tests/matched_outputs/"${filename_noext}_my_output.txt"
    cp output_gcc.txt tests/matched_outputs/"${filename_noext}_gcc_output.txt"

else
    echo "❌ Outputs differ! Check the diff above."
fi


# Optional: Cleanup temporary outputs
# rm output_my_compiler.txt output_gcc.txt
