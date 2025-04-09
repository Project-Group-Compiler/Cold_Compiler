#include "asm.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

std::ofstream asm_file;

void emit_instr(const std::string &instr)
{
    // asm_file << instr << "\n";
    std::cout << instr << "\n";
}

void emit_asm(const std::string &inputFile)
{
    // std::ofstream asm_file(outputDir + inputFile + ".asm");
    // if (!asm_file)
    // {
    //     print_error("cannot open " + outputDir + inputFile + ".asm");
    //     return;
    // }
    emit_instr(x86_lib::xor_op("eax", "eax"));
}