# Cold Compiler

An end-to-end compiler for a new custom programming language, `Cold`.

`Cold` has a C-like syntax based on ANSI C.

- Source language : `Cold` (C-like but based on an older version, hence C-old)
- Implementation language : `C++`
- Intermediate Representaion : `3AC`
- Target Language : `x86_32` assembly

The C Lex specification is taken from [Here](https://www.lysator.liu.se/c/ANSI-C-grammar-l.html)

For learning ANSI C syntax, the perfect book obviously is K&R C Book.

## Installation

Run for Ubuntu/Linux
```
sudo apt install flex bison build-essential
```

## Running the Compiler

1. Clone the Repository
```
git clone https://github.com/Project-Group-Compiler/Cold_Compiler.git
cd Cold_Compiler
```

2. Build the Compiler
   
Run the following command in the root directory:
```
make
```

3. Run script for testcases

```
chmod +x run.sh
./run.sh bin/compiler
```
   
## Team Members

- Meet Sindhav
- Mohammed Haaziq
- Roopam Taneja
- Shubham Kumar Verma

### Made with ❤️
