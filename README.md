# Cold Compiler

An end-to-end compiler for a new custom programming language, **`Cold`**.

`Cold` has a C-like syntax based on ANSI C and also supports some object-oriented features.

Regarding the name of our language, since it is C-like but based on an older version, hence ***`C-old`***. Also since some object-oriented features are supported, hence `Cold` can also be thought of as "`C` but with `O`bject-oriented `L`anguage `D`esign features".

- Source language : `Cold`
- Implementation language : `C++`
- Intermediate Representaion : `3AC`
- Target Language : `x86 32-bit`

For learning ANSI C syntax, the perfect book obviously is K&R C Book.

The C Lex specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-l.html).

The C Yacc specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html).

Syntax reference for x86 assembly has been mainly taken from [here](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html). For floating-point operations, x87 instruction set has been used whose reference has taken mainly from [here](https://people.hsc.edu/faculty-staff/robbk/Coms480/Lectures/Spring%202009/Lecture%2018%20-%20The%20x87%20FPU.pdf).

All basic features required have been supported (*including structs, library functions and static*).

**Advanced features** :

- recursive function call
- classes and objects *(including '.' and '->' access)*
- inheritance
- function call with variable arguments *(including va_list, va_start, va_end)*
- dynamic memory allocation *(and other standard library functions)*
- command line input
- public, private and protected keywords
- typedef
- reference
- enum, union
- file manipulation
- until loop
- multi-level pointers
- multi-dimensional arrays

**Extra Advanced Features** :

- function overloading 
- const type qualifier
- multi-level inheritance
- modified operator precedence (more details in `docs/operator_precedence.md`.)

**Extra Optimizations** :

- *Machine-independent optimizations* :
  1. Constant folding
  2. Dead code elimination
  3. Constant propagation
  4. Strength reduction 
- *Machine-dependent optimizations*:
  1. Efficient register allocation using next use information and register descriptors
  2. Peephole optimizations (like strength reduction and algebraic simplification)
  3. Removing redundant stores and assignments
  4. Efficient spilling of registers (via minimum number of live variable heuristic)

Some syntax changes and implementation related details have been documented in `docs/lang_details.md`.

`Error handling` : Multiple errors in same program can be handled (shown in testcases) and descriptive error messages are being printed in terminal.

## Installation

Run for Ubuntu/Linux. Make sure you have `g++` installed.

```bash
sudo apt install flex bison build-essential nasm
git clone https://github.com/Project-Group-Compiler/Cold_Compiler.git
cd Cold_Compiler
```

## Running the compiler

1. Build the compiler
   
    Run the following command in the root directory:

    ```bash
    make
    ```

2. Run script for executing testcases. The generated assembly code will get saved in `outputs` directory.

    ```bash
    chmod +x run.sh
    ./run.sh
    ```

Alternatively, generate assembly for a single input file using (specify input file as the first argument):

```bash
bin/compiler <input_file> [options]
```

```markdown
Extra Options:
    -h, --help       Show this help message and exit
    -l, --lex        Print lexical analysis table
    -a, --ast        Print abstract syntax tree as dot file
    -s, --symtab     Print symbol tables
    -d, --debug      Print debug trace
    -t, --tac        Print three address code
    -f, --force      Forcefully continue even if errors are present
    -O1              Enable machine-independent optimizations
    -O2              Enable machine-dependent optimizations
    -O3              Enable all optimizations
```

## To create executable : 

```bash
nasm -f elf32 <file_name>.asm                                # for assembling
gcc -m32 -no-pie -z noexecstack <file_name>.o -o <some_name> # for linking
```

**Shortcut** : 

```bash
chmod +x coldc.sh
./coldc.sh <input_file> [cold-compiler-options]
```
   
## Team Members

- Meet Sindhav
- Mohammed Haaziq
- Roopam Taneja
- Shubham Kumar Verma

### Made with ❤️
