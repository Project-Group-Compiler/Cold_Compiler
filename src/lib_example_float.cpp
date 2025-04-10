// Example of how to use the x87 FPU instructions from the library

#include "x86_32_lib.hpp"
#include <iostream>

// can assemble

/*for code :
float factorial(float n)
{
    float result = 1.0;
    while (n > 1.0)
    {
        result *= n;
        n -= 1.0;
    }
    return result;
}

int main()
{
    float input = 6.0;
    float fact_result = factorial(input);
    return 0;
}
*/

void emit_label(const std::string &label)
{
    std::cout << label << ":\n";
}

void emit_instr(const std::string &instr)
{
    std::cout << "\t" << instr << "\n";
}

void emit_data(const std::string &data)
{
    std::cout << "\t" << data << "\n";
}

void emit_section(const std::string &section)
{
    std::cout << "section " << section << "\n";
}

int main()
{
    // Data section first
    emit_section(".data");
    emit_label("input");
    emit_data("dd 6.0");

    // Code section
    emit_section(".text");
    emit_data("global main");

    // Factorial function
    emit_label("factorial");
    emit_instr(x86_lib::push("ebp"));
    emit_instr(x86_lib::mov("ebp", "esp"));
    emit_instr(x86_lib::sub("esp", "16"));

    // Initialize result to 1.0
    emit_instr(x86_lib::fld1());
    emit_instr(x86_lib::fstp_mem("dword", "ebp-4"));

    // Jump to condition check
    emit_instr(x86_lib::jmp("L2"));

    // Loop body
    emit_label("L3");

    // result *= n
    emit_instr(x86_lib::fld_mem("dword", "ebp-4"));  // Load result
    emit_instr(x86_lib::fld_mem("dword", "ebp+8"));  // Load n
    emit_instr(x86_lib::fmulp("st1"));               // result *= n (correct NASM syntax)
    emit_instr(x86_lib::fstp_mem("dword", "ebp-4")); // Store result

    // n -= 1.0
    emit_instr(x86_lib::fld_mem("dword", "ebp+8"));  // Load n
    emit_instr(x86_lib::fld1());                     // Load 1.0
    emit_instr(x86_lib::fsubp("st1"));               // n -= 1.0 (correct NASM syntax)
    emit_instr(x86_lib::fstp_mem("dword", "ebp+8")); // Store n

    // Loop condition check
    emit_label("L2");
    emit_instr(x86_lib::fld1());                    // Load 1.0
    emit_instr(x86_lib::fld_mem("dword", "ebp+8")); // Load n
    emit_instr(x86_lib::fcomip("st1"));             // Compare n with 1.0 (correct NASM syntax)
    emit_instr(x86_lib::fstp("st0"));               // Pop 1.0 (correct NASM syntax)
    emit_instr(x86_lib::ja("L3"));                  // If n > 1.0, continue loop

    // Return result
    emit_instr(x86_lib::fld_mem("dword", "ebp-4")); // Load result
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());

    // Main function
    emit_label("main");
    emit_instr(x86_lib::push("ebp"));
    emit_instr(x86_lib::mov("ebp", "esp"));
    emit_instr(x86_lib::sub("esp", "16"));

    // Initialize input to 6.0
    emit_instr(x86_lib::fld_mem("dword", "input"));  // Load 6.0 from data section
    emit_instr(x86_lib::fstp_mem("dword", "ebp-4")); // Store input

    // Call factorial
    emit_instr(x86_lib::push_mem("dword", "ebp-4")); // Push input
    emit_instr(x86_lib::call("factorial"));
    emit_instr(x86_lib::add("esp", "4")); // Clean up parameter

    // Store result
    emit_instr(x86_lib::fstp_mem("dword", "ebp-8")); // Store factorial result

    // Return 0
    emit_instr(x86_lib::mov("eax", "0"));
    emit_instr(x86_lib::leave());
    emit_instr(x86_lib::ret());

    return 0;
}