For now, understanding workflow and creating directory structure

Patterns go in lexer.l

Lang grammar in parser.y

To generate cpp scanner and parser files use :

1) cd src
2) bison -d parser.y -o parser.cpp
3) flex -o lexer.cpp lexer.l
4) g++ lexer.cpp compiles and gives our scanner

TODO:

- Understand grammar and populate lexer.l and parser.y
- Use a makefile/cmake
- Write tests