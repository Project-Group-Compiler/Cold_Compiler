#include <iostream>
#include <fstream>
#include "tac.hpp"
#include "data_structures.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

void print_tac_code(const std::string &inputFile)
{
    std::ofstream out(outputDir + inputFile + "_IR.txt");
    if (!out)
    {
        print_error("cannot open " + outputDir + inputFile + "_IR.txt");
        return;
    }

    out << "Three Address Code:\n\n";
    // TODO : write printing functions
}