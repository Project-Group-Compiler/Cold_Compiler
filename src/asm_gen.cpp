#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

std::ofstream asm_file;

void emit_asm(const std::string &inputFile)
{
    if (!optimize_ir)
        addgotoLabels();

    asm_file = std::ofstream(outputDir + inputFile + ".asm");
    if (!asm_file)
    {
        print_error("cannot open " + outputDir + inputFile + ".asm");
        return;
    }

    compute_basic_blocks();
    emit_section(".data");
    // add initialized data
    emit_section(".bss");
    // add uninitialized data
    emit_section(".text");
    emit_data("global main");
    add_extern_funcs();

    for (auto &block : basic_blocks)
    {
        next_use_analysis(block);
        for (auto &instr : block)
        {
            const std::string curr_op = instr.op;
            if (curr_op.substr(0, 5) == "FUNC_")
            {
                if (curr_op.substr(curr_op.length() - 7) == "start :")
                {
                    if (curr_op.substr(5, 5) == "4main")
                        emit_label("\nmain ");
                    else
                        emit_label("\n" + curr_op.substr(0, curr_op.length() - 7));
                    // TODO : add function prologue
                }
                // no code emitted for func_end
            }
            else if (curr_op.back() == ':')
                emit_label(curr_op.substr(0, curr_op.length() - 1));
        }
    }
}

void add_extern_funcs()
{
    for (auto &func : called_lib_funcs)
        emit_data("extern " + func);
    asm_file << "\n";
    // std::cout << "\n";
}

void next_use_analysis(std::vector<quad> &block)
{
    // TODO after pairs made
}
