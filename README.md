# Cold Compiler

An end-to-end compiler for a new custom programming language, **`Cold`**.

`Cold` has a C-like syntax based on ANSI C and also supports some object-oriented features.

Regarding the name of our language, since it is C-like but based on an older version, hence ***`C-old`***. Also since some object-oriented features are supported, hence `Cold` can also be thought of as "`C` but with `O`bject-oriented `L`anguage `D`esign features".

- Source language : `Cold`
- Implementation language : `C++`
- Intermediate Representaion : `3AC`
- Target Language : `x86_32` assembly

For learning ANSI C syntax, the perfect book obviously is K&R C Book.

The C Lex specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-l.html).

The C Yacc specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html).

Syntax reference for x86 assembly has been mainly taken from [here](https://www.cs.virginia.edu/~evans/cs216/guides/x86.html). For floating-point operations, x87 instruction set has been used whose reference has taken mainly from [here](https://people.hsc.edu/faculty-staff/robbk/Coms480/Lectures/Spring%202009/Lecture%2018%20-%20The%20x87%20FPU.pdf).

<!-- All basic features required have been supported (*including structs, library functions and static*). -->

<!-- **List of advanced features supported apart from basic features** : -->

<!-- - recursive function call
- classes and objects
- inheritance
- function call with variable arguments
- dynamic memory allocation
- command line input
- public, private and protected keywords
- typedef
- enum, union
- file manipulation
- until loop
- multi-level pointers
- multi-dimensional arrays -->

<!-- **Extra advanced features** : -->

<!-- - function overloading 
- const type qualifier
- multi-level inheritance
- modified operator precedence (more details in `docs/operator_precedence.md`.)
- *machine-independent optimizations* implemented (at this point) :
  1. constant folding -->
<!--  2. dead code elimination -->

Some syntax changes and implementation related details have been documented in `docs/lang_details.md`.

`Error handling` : Multiple errors in same program can be handled (shown in testcases) and descriptive error messages are being printed in terminal.

## Installation

Run for Ubuntu/Linux. Make sure you have `g++` installed.

```bash
sudo apt install flex bison build-essential graphviz
git clone https://github.com/Project-Group-Compiler/Cold_Compiler.git
cd Cold_Compiler
```

## Running the compiler

1. Build the compiler
   
    Run the following command in the root directory:

    ```bash
    make
    ```

2. Run script for executing testcases. run.sh still saves IR for time being.

    ```bash
    chmod +x run.sh
    ./run.sh
    ```

**Having int main() is mandatory.**

Alternatively, generate assembly for a single input file using:

```bash
bin/compiler <input_file> [options]
```

```markdown
Extra Options:
    -h, --help       Show this help message and exit
    -O0              Generate unoptimized IR
    -l, --lex        Print lexical analysis table
    -a, --ast        Print abstract syntax tree as dot file
    -s, --symtab     Print symbol tables
    -d, --debug      Print debug trace
    -t, --tac        Print three address code
    -f, --force      Forcefully continue even if errors are present
```

## To create executable : 

Install nasm if not already installed.

```bash
nasm -f elf32 <file_name>.asm # to assemble
gcc -m32 -no-pie -z noexecstack <file_name>.o -o <some_name> # to create object file
```

-o flag is optional ofcourse.

this will create binary "some_name"
run using ./some_name.


**Shortcut** : 

```bash

chmod +x coldc.sh

./coldc.sh <input_file> <any_other_cold_compiler_flags>
```

Two important things to note : 

1. Specify input file as the first argument
2. Don't specify -h here


Optionally, convert AST DOT file into image using:

```bash
dot -Tpng <dot_file> -o <png_file> 
```
   
## Team Members

- Meet Sindhav
- Mohammed Haaziq
- Roopam Taneja
- Shubham Kumar Verma

### Made with ❤️
