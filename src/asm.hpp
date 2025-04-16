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
    for (auto &func : called_lib_funcs)
        asm_file << "extern " << func << "\n";
}

void emit_asm(const std::string &);
void emit_fn_defn(quad &instr);
void emit_assign(quad &instr);
void emit_add(quad &instr);
void emit_sub(quad &instr);

void next_use_analysis(std::vector<quad> &block);
void print_next_use();

void update_ir();
void emit_data_section();
void emit_bss_section();

#endif