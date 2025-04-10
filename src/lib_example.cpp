// Example of how to use the library

// Generates all reqd instructions
// except extern, section, data in the end etc

// Don't assemble this

#include "x86_32_lib.hpp"
#include <iostream>

void emit_label(const std::string &label)
{
    std::cout << label << ":\n";
}

void emit_instr(const std::string &instr)
{
    std::cout << "\t" << instr << "\n";
}

/*Code example :

int main()
{
    int n = 6;
    int result = 1;
    if (n <= 1)
        return result;
    while (n > 1)
    {
        result *= n;
        n--;
    }
    printf("%d\n", result);
    return 0;
}

*/

int main()
{
    // L0 label
    emit_label("L0");

    // main function
    emit_label("main");
    emit_instr(x86_lib::push("ebp"));
    emit_instr(x86_lib::mov("ebp", "esp"));
    emit_instr(x86_lib::sub("esp", "28"));
    emit_instr(x86_lib::mov_mem_imm("dword", "ebp-4", "6"));
    emit_instr(x86_lib::mov_mem_imm("dword", "ebp-8", "1"));
    emit_instr(x86_lib::mov_reg_mem("eax", "ebp-4"));
    emit_instr(x86_lib::cmp("eax", "1"));
    emit_instr(x86_lib::jle("L12"));
    emit_instr(x86_lib::mov_reg_imm("eax", "0"));
    emit_instr(x86_lib::jmp("L13"));

    // L12 label
    emit_label("L12");
    emit_instr(x86_lib::mov_reg_imm("eax", "1"));

    // L13 label
    emit_label("L13");
    emit_instr(x86_lib::mov_mem_reg("ebp-12", "eax"));
    emit_instr(x86_lib::cmp_mem_imm("dword", "ebp-12", "0"));
    emit_instr(x86_lib::jne("L1"));

    // L2 label
    emit_label("L2");
    emit_instr(x86_lib::jmp("L3"));

    // L1 label
    emit_label("L1");
    emit_instr(x86_lib::mov_reg_mem("eax", "ebp-8"));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());

    // L3 label
    emit_label("L3");
    emit_instr(x86_lib::mov_reg_mem("eax", "ebp-4"));
    emit_instr(x86_lib::cmp("eax", "1"));
    emit_instr(x86_lib::jg("L14"));
    emit_instr(x86_lib::mov_reg_imm("eax", "0"));
    emit_instr(x86_lib::jmp("L15"));

    // L14 label
    emit_label("L14");
    emit_instr(x86_lib::mov_reg_imm("eax", "1"));

    // L15 label
    emit_label("L15");
    emit_instr(x86_lib::mov_mem_reg("ebp-16", "eax"));
    emit_instr(x86_lib::cmp_mem_imm("dword", "ebp-16", "0"));
    emit_instr(x86_lib::jne("L5"));

    // L6 label
    emit_label("L6");
    emit_instr(x86_lib::jmp("L7"));

    // L5 label
    emit_label("L5");
    emit_instr(x86_lib::mov_reg_mem("eax", "ebp-8"));
    emit_instr(x86_lib::imul_reg_mem("eax", "ebp-4"));
    emit_instr(x86_lib::mov_reg_mem("ebx", "ebp-4"));
    emit_instr(x86_lib::mov("ecx", "ebx"));
    emit_instr(x86_lib::dec("ebx"));
    emit_instr(x86_lib::mov_mem_reg("ebp-4", "ebx"));
    emit_instr(x86_lib::mov_mem_reg("ebp-20", "eax"));
    emit_instr(x86_lib::mov_mem_reg("ebp-8", "eax"));
    emit_instr(x86_lib::mov_mem_reg("ebp-4", "ebx"));
    emit_instr(x86_lib::mov_mem_reg("ebp-24", "ecx"));
    emit_instr(x86_lib::jmp("L3"));

    // L7 label
    emit_label("L7");
    emit_instr(x86_lib::push_mem("dword", "ebp-8"));
    emit_instr(x86_lib::push("__str__0"));
    emit_instr(x86_lib::call("printf"));
    emit_instr(x86_lib::add("esp", "8"));
    emit_instr(x86_lib::mov_reg_imm("eax", "0"));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());
    emit_instr(x86_lib::mov_mem_reg("ebp-28", "eax"));
    emit_instr(x86_lib::xor_op("eax", "eax"));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());

    // L11 label
    emit_label("L11");
    return 0;
}
