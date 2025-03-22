#include <iostream>
#include <filesystem>
#include "data_structures.hpp"
#include "symbol_table.h"

extern FILE *yyin;
bool print_ast = false;
std::string inputFilename;
std::string outputDir = std::filesystem::current_path().string() + "/";
std::string astDir = std::filesystem::current_path().string() + "/";

bool has_error = false;
void print_error(const std::string &message)
{
    std::cerr << "\033[1;31merror: \033[0m" << message << "\n";
}

void performLexing(const std::string &inputFile, bool lexPrint);
void performParsing(const std::string &inputFile);

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        print_error("no arguments provided.");
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            std::cout << "Usage: driver <input_file> [options]\n"
                      << "Options:\n"
                      << "  -h, --help       Show this help message and exit\n"
                      << "  -l, --lex        Print lexical analysis table\n"
                      << "  -a, --ast        Print abstract syntax tree\n"
                      << "  -s, --symtab     Print symbol tables\n"
                      << "  -f, --force      Force parsing even if lexical errors are present\n";
            return 0;
        }
    }

    yyin = fopen(argv[1], "r");
    if (!yyin)
    {
        print_error("cannot open " + std::string(argv[1]));
        return -1;
    }
    inputFilename = argv[1];

    std::string inputFileString = std::filesystem::path(inputFilename).stem().string();
    bool lexPrint = false;
    bool print_symtab = false;
    bool force = false;
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--lex")
            lexPrint = true;
        else if (arg == "-a" || arg == "--ast")
            print_ast = true;
        else if (arg == "-s" || arg == "--symtab")
            print_symtab = true;
        else if (arg == "-f" || arg == "--force")
            force = true;
        else if (arg == "-o" || arg == "--output")
        {
            if (i + 1 < argc)
                outputDir = argv[++i];
            else
            {
                print_error("no output directory provided.");
                return -1;
            }
        }
        else if (arg == "--astdir")
        {
            if (i + 1 < argc)
                astDir = argv[++i];
            else
            {
                print_error("no ast directory provided.");
                return -1;
            }
        }
        else
        {
            print_error("unknown option " + arg);
            return -1;
        }
    }

    performLexing(inputFileString, lexPrint);

    if (!has_error || force)
    {
        symTable_init();//mohitmo's function
        performParsing(inputFileString);
        if (print_symtab){
            std::cout << "Printing symbol table\n";
            printTables(inputFileString);//meet's function
            printSymbolTable(&gst, "#Global_Symbol_Table#.csv");//mohitmo's function
        }
            
    }
    else
        print_error("\nlexical errors present, use -f to force parsing");

    fclose(yyin);
    if (has_error)
        return 1;
    return 0;
}
