#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

std::ofstream asm_file;

void emit_instr(const std::string &instr)
{
    // asm_file << "\t" << instr << "\n";
    std::cout << "\t" << instr << "\n";
}

void emit_label(const std::string &label)
{
    // asm_file << label << ":\n";
    std::cout << label << ":\n";
}

void emit_data(const std::string &data)
{
    // asm_file << "\t" << data << "\n";
    std::cout << "\t" << data << "\n";
}

void emit_section(const std::string &section)
{
    // asm_file << "section " << section << "\n";
    std::cout << "section " << section << "\n";
}

void add_extern_funcs()
{
    for (auto &func : called_lib_funcs)
    {
        // asm_file << "extern " << func << "\n";
        std::cout << "extern " << func << "\n";
    }
}

void emit_asm(const std::string &inputFile)
{
    if (!optimize_ir)
        addgotoLabels();

    // asm_file = std::ofstream(outputDir + inputFile + ".asm");
    // if (!asm_file)
    // {
    //     print_error("cannot open " + outputDir + inputFile + ".asm");
    //     return;
    // }

    compute_basic_blocks();
    add_extern_funcs();
    emit_section(".text");
    emit_data("global main");
}
