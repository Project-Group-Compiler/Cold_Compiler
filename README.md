# Cold Compiler

An end-to-end compiler for a new custom programming language, **`Cold`**.

`Cold` has a C-like syntax based on ANSI C and also supports some object-oriented features.

Regarding the name of our language, since it is C-like but based on an older version, hence *C-old*. Also since some object-oriented features are supported, hence `Cold` can also be thought of as "**C** but with **O**bject-oriented **L**anguage **D**esign features".

- Source language : `Cold`
- Implementation language : `C++`
- Intermediate Representaion : `3AC`
- Target Language : `x86_32` assembly

For learning ANSI C syntax, the perfect book obviously is K&R C Book.

The C Lex specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-l.html).

The C Yacc specification is taken from [here](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html).

**List of advanced features supported apart from basic features** :

- recursive function call
- classes and objects
- inheritance (including multiple inheritance)
- dynamic memory allocation
- command line input
- public, private and protected keywords
- typedef for primitive data types
- enum, union and struct
- file manipulation
- until loop
- multi-level pointers
- multi-dimensional arrays
- const, signed, unsigned, extern and static

Additionally, since we are crafting a new language, we have taken the liberty to slightly *modify normal C operator precedence*. More details in `docs/operator_precedence.md`.

<!-- **Note:** Errors are printed in terminal. Only invalid symbols not part of ANSI C are handled at this point. Rest of the errors will be handled in subsequent stages. -->

## Installation

Run for Ubuntu/Linux. Make sure you have `g++` installed.

```bash
sudo apt install flex bison build-essential graphviz
git clone https://github.com/Project-Group-Compiler/Cold_Compiler.git
cd Cold_Compiler
```

## Running the parser

1. Build the parser
   
    Run the following command in the root directory:

    ```bash
    make
    ```

2. Run script for executing testcases. Output symbol tables get saved in `outputs` directory. ASTs are also generated which get saved in `ast` directory.

    ```bash
    chmod +x run.sh
    ./run.sh
    ```

Alternatively, execute for a single input file using:

```bash
bin/parser <input_file> [options]
```

```markdown
Options:
    -h, --help       Show this help message and exit
    -l, --lex        Print lexical analysis table
    -a, --ast        Print abstract syntax tree as dot file
    -s, --symtab     Print symbol tables
    -f, --force      Force parsing even if lexical errors are present
```

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
