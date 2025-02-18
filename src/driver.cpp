#include <iostream>
#include <filesystem>

extern FILE *yyin;
bool print_ast = false;
std::string inputFilename;
std::string outputDir = std::filesystem::current_path().string() + "/";


bool has_error = false;
void print_error(const std::string &message)
{
    std::cerr << "\033[1;31merror: \033[0m" << message << "\n";
}

void print_options()
{
    std::cout << "Usage: driver <input_file> [options]\n"
              << "Options:\n"
              << "  -h, --help       Show this help message and exit\n"
              << "  -l, --lex        Print lexical analysis table\n"
              << "  -a, --ast        Print abstract syntax tree\n"
              << "  -f, --force      Force parsing even if lexical errors are present\n";
}

void printLexTable(const std::string &inputFile);
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
            print_options();
            return 0;
        }
    }

    yyin = fopen(argv[1], "r");
    inputFilename = argv[1];
    std::string inputFileString = std::filesystem::path(inputFilename).stem().string();
    if (!yyin)
    {
        print_error("cannot open " + std::string(argv[1]));
        return -1;
    }
    bool lexPrint = false;
    bool force = false;
    for (int i = 2; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-l" || arg == "--lex")
            lexPrint = true;
        if (arg == "-a" || arg == "--ast")
            print_ast = true;
        if (arg == "-f" || arg == "--force")
            force = true;
        if (arg == "-o" || arg == "--output")
        {
            if (i + 1 < argc)
                outputDir = argv[++i];
            else
            {
                print_error("no output directory provided.");
                return -1;
            }
        }
    }

    if (lexPrint)
        printLexTable(inputFileString);

    if (!has_error || force)
        performParsing(inputFileString);

    fclose(yyin);
    return 0;
}
