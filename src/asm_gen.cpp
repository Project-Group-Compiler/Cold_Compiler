#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

std::ofstream asm_file;

void emit_asm(const std::string &inputFile)
{
    if (tac_code.empty())
        return;
    if (!optimize_ir)
        addgotoLabels();

    asm_file = std::ofstream(outputDir + inputFile + ".asm");
    if (!asm_file)
    {
        print_error("cannot open " + outputDir + inputFile + ".asm");
        return;
    }

    get_string_literals();
    compute_basic_blocks();

    add_extern_funcs();
    emit_section(".data");
    print_global_data(); // add initialized data
    emit_section(".bss");
    // add uninitialized data
    emit_section(".text");
    emit_data("global main");

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

std::map<std::string, int> string_literals;
void get_string_literals()
{
    int str_cnt = 0;
    for (auto &instr : tac_code)
    {
        if (instr.arg1.value.length() > 0 && instr.arg1.value[0] == '\"')
        {
            if (string_literals.find(instr.arg1.value) == string_literals.end())
                string_literals[instr.arg1.value] = str_cnt++;
            instr.arg1.value = "__str_" + std::to_string(string_literals[instr.arg1.value]);
        }
        if (instr.arg2.value.length() > 0 && instr.arg2.value[0] == '\"')
        {
            if (string_literals.find(instr.arg2.value) == string_literals.end())
                string_literals[instr.arg2.value] = str_cnt++;
            instr.arg2.value = "__str_" + std::to_string(string_literals[instr.arg2.value]);
        }
        if (instr.result.value.length() > 0 && instr.result.value[0] == '\"')
        {
            if (string_literals.find(instr.result.value) == string_literals.end())
                string_literals[instr.result.value] = str_cnt++;
            instr.result.value = "__str_" + std::to_string(string_literals[instr.result.value]);
        }
    }
}

// print the data section for the asm file
void print_global_data()
{
    // for (auto &[name, entry] : gst)
    // {
    // }

    //     if (!globaldecl.empty())
    //     {
    //         code_file << "section .data\n";
    //         for (auto it : globaldecl)
    //         {
    //             if (it.second.second == 0)
    //                 code_file << "\t" << it.first << get_type_size(it.first) << it.second.first << "\n";
    //             else
    //             {
    //                 if (global_array_init_map.find(it.first) == global_array_init_map.end())
    //                     code_file << "\t" << it.first << " times " << it.second.second << get_type_size(it.first) << it.second.first << "\n";
    //                 else
    //                 {
    //                     code_file << "\t" << it.first << get_type_size(it.first);
    //                     vector<qid> temp = global_array_init_map[it.first];
    //                     reverse(temp.begin(), temp.end());
    //                     int i;
    //                     for (i = 0; i < temp.size() - 1; i++)
    //                     {
    //                         temp[i].first = char_to_int(temp[i].first);
    //                         if (is_integer(temp[i].first) || temp[i].first[0] == '\"')
    //                             code_file << temp[i].first << ", ";
    //                         else
    //                             code_file << "0, ";
    //                     }
    //                     i = temp.size() - 1;
    //                     temp[i].first = char_to_int(temp[i].first);
    //                     if (is_integer(temp[i].first) || temp[i].first[0] == '\"')
    //                         code_file << temp[i].first;
    //                     else
    //                         code_file << "0";

    //                     int extra_elements = it.second.second - temp.size();
    //                     while (extra_elements > 0)
    //                     {
    //                         extra_elements--;
    //                         code_file << ", 0";
    //                     }
    //                     code_file << "\n";
    //                 }
    //             }
    //         }
    //     }
    for (auto i = 0; i < string_literals.size(); i++)
    {
        for (const auto &[str, cnt] : string_literals)
        {
            if (cnt == i)
                emit_data("__str_" + std::to_string(cnt) + ": db " + str + ", 0\n");
        }
    }
}

void next_use_analysis(std::vector<quad> &block)
{
    // TODO after pairs made
}
