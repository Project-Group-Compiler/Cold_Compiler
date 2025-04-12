#ifndef ASM_GEN_HPP
#define ASM_GEN_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "tac.hpp"

extern std::ofstream asm_file;

inline void emit_instr(const std::string &instr)
{
    asm_file << "\t" << instr << "\n";
}

inline void emit_label(const std::string &label)
{
    asm_file << label << ":\n";
}

inline void emit_data(const std::string &data)
{
    asm_file << "\t" << data << "\n";
}

inline void emit_section(const std::string &section)
{
    asm_file << "\nsection " << section << "\n";
}

inline void add_extern_funcs()
{
    if (called_lib_funcs.empty())
        return;
    for (auto &func : called_lib_funcs)
        asm_file << "extern " << func << "\n";
}

void emit_asm(const std::string &);

void next_use_analysis(std::vector<quad> &block);

void print_data_section();
void print_bss_section();
void get_string_literals();

void emit_assign(quad &instr);

#endif