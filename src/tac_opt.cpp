#include "tac.hpp"
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <cmath>

bool optimize_ir = true;

std::vector<std::vector<quad>> basic_blocks;

void compute_basic_blocks()
{
    basic_blocks.clear();
    std::vector<quad> block;
    for (auto &instr : tac_code)
    {
        const std::string &curr_op = instr.op;
        if (curr_op.back() == ':')
        {
            if (!block.empty())
            {
                basic_blocks.push_back(block);
                block.clear();
            }
            block.push_back(instr);
        }
        else if ((curr_op == "GOTO") || curr_op == "RETURN" || (curr_op.substr(0, 5) == "FUNC_" && curr_op.substr(curr_op.length() - 3) == "end"))
        {
            block.push_back(instr);
            basic_blocks.push_back(block);
            block.clear();
        }
        else
            block.push_back(instr);
    }
    if (!block.empty())
        basic_blocks.push_back(block);
}

void print_basic_blocks(bool modifygotoLabels)
{
    int block_no = 0;
    for (auto &block : basic_blocks)
    {
        std::cout << "Block " << block_no++ << ":\n";
        for (auto &instr : block)
        {
            std::cout << stringify(instr, modifygotoLabels) << std::endl;
        }
        std::cout << "\n";
    }
}

void blocks_to_code()
{
    if (basic_blocks.empty())
        return;
    std::vector<quad> new_tac_code;
    std::unordered_map<int, int> id_map;
    for (auto &block : basic_blocks)
    {
        if (!block.empty())
        {
            for (auto &instr : block)
            {
                int newid = new_tac_code.size();
                id_map[instr.Label] = newid;
                instr.Label = newid;
                new_tac_code.push_back(std::move(instr));
            }
        }
    }
    for (auto &instr : new_tac_code)
    {
        if (instr.gotoLabel != -1)
            instr.gotoLabel = id_map[instr.gotoLabel];
    }
    tac_code = std::move(new_tac_code);
}

struct FunctionCFG
{
    std::string name;
    std::vector<std::vector<quad>> blocks;
    std::unordered_map<int, int> first_map;
    std::vector<std::vector<int>> adj;
    std::vector<std::vector<int>> rev_adj;
};

std::vector<FunctionCFG> function_cfgs;

void separate_functions()
{
    function_cfgs.clear();
    FunctionCFG current_function;
    bool in_function = false;

    for (auto i = 0; i < basic_blocks.size(); i++)
    {
        const auto &block = basic_blocks[i];
        if (block.empty())
            continue;

        if (block[0].op.substr(0, 5) == "FUNC_" && block[0].op.substr(block[0].op.length() - 7) == "start :")
        {
            if (in_function && !current_function.blocks.empty())
                function_cfgs.push_back(std::move(current_function));

            current_function = FunctionCFG();
            current_function.name = block[0].op.substr(0, block[0].op.length() - 7);
            current_function.blocks.push_back(block);
            in_function = true;
        }
        else if (block.back().op.substr(0, 5) == "FUNC_" && block.back().op.substr(block.back().op.length() - 3) == "end")
        {
            current_function.blocks.push_back(block);
            function_cfgs.push_back(std::move(current_function));
            in_function = false;
        }
        else if (in_function)
        {
            current_function.blocks.push_back(block);
        }
        else // Global code outside function
        {
            FunctionCFG global_code;
            global_code.name = "global";
            global_code.blocks.push_back(block);
            function_cfgs.push_back(std::move(global_code));
        }
    }

    if (in_function && !current_function.blocks.empty())
        function_cfgs.push_back(std::move(current_function));

    for (auto &func : function_cfgs)
        for (auto i = 0; i < func.blocks.size(); i++)
            if (!func.blocks[i].empty())
                func.first_map[func.blocks[i][0].Label] = i;
}

void print_separate_functions()
{
    for (const auto &func : function_cfgs)
    {
        std::cout << "Function: " << func.name << "\n";
        for (const auto &block : func.blocks)
        {
            std::cout << "Block:\n";
            for (const auto &instr : block)
                std::cout << stringify(instr) << "\n";
        }
        std::cout << "\n";
    }
}

// Build CFG for a single function
void build_function_cfg(FunctionCFG &func)
{
    if (func.blocks.empty())
        return;

    func.adj.clear();
    func.rev_adj.clear();
    func.adj.resize(func.blocks.size());
    func.rev_adj.resize(func.blocks.size());

    for (auto i = 0; i < func.blocks.size(); i++)
    {
        const auto &block = func.blocks[i];
        if (block.empty())
            continue;

        const std::string &last_op = block.back().op;
        if (last_op == "GOTO")
        {
            auto it = func.first_map.find(block.back().gotoLabel);
            if (it != func.first_map.end())
            {
                int target_block = it->second;
                func.adj[i].push_back(target_block);
                func.rev_adj[target_block].push_back(i);

                if (block.back().arg1.value == "IF")
                {
                    if (i + 1 < func.blocks.size() && i + 1 != target_block)
                    {
                        func.adj[i].push_back(i + 1);
                        func.rev_adj[i + 1].push_back(i);
                    }
                }
            }
        }
        else if (last_op == "RETURN")
        {
            if (func.blocks.back().empty())
                continue;
            std::string back_op = func.blocks.back().back().op;
            if (back_op.substr(0, 5) == "FUNC_" && back_op.substr(back_op.length() - 3) == "end")
            {
                func.adj[i].push_back(func.blocks.size() - 1);
                func.rev_adj[func.blocks.size() - 1].push_back(i);
            }
        }
        else if (last_op.substr(0, 5) != "FUNC_" || last_op.substr(last_op.length() - 3) != "end")
        {
            if (i + 1 < func.blocks.size())
            {
                func.adj[i].push_back(i + 1);
                func.rev_adj[i + 1].push_back(i);
            }
        }
    }
}

void reconstruct_basic_blocks()
{
    if (function_cfgs.empty())
        return;
    basic_blocks.clear();
    for (const auto &func : function_cfgs)
        for (const auto &block : func.blocks)
            basic_blocks.push_back(block);
}

void build_cfg()
{
    separate_functions();
    for (auto &func : function_cfgs)
        build_function_cfg(func);
}

void print_function_cfg(const FunctionCFG &func)
{
    std::cout << "Control Flow Graph for function " << func.name << ":\n";
    std::cout << "---------------------------------------\n";
    for (auto i = 0; i < func.adj.size(); i++)
    {
        std::cout << "Block " << i << ":\n";
        for (const auto &instr : func.blocks[i])
            std::cout << "  " << stringify(instr) << "\n";

        std::cout << "Successors: ";
        if (func.adj[i].empty())
            std::cout << "none";
        else
        {
            for (auto j = 0; j < func.adj[i].size(); j++)
            {
                std::cout << func.adj[i][j];
                if (j < func.adj[i].size() - 1)
                    std::cout << ", ";
            }
        }
        std::cout << "\n";

        std::cout << "Predecessors: ";
        if (func.rev_adj[i].empty())
            std::cout << "none";
        else
        {
            for (auto j = 0; j < func.rev_adj[i].size(); j++)
            {
                std::cout << func.rev_adj[i][j];
                if (j < func.rev_adj[i].size() - 1)
                    std::cout << ", ";
            }
        }
        std::cout << "\n\n";
    }
}

void print_cfg()
{
    for (const auto &func : function_cfgs)
        print_function_cfg(func);
}

void constant_folding()
{
    std::vector<int> id_map(tac_code.size(), -1);
    std::vector<quad> folded_tac_code;
    for (quad &instr : tac_code)
    {
        bool add_instr = true;
        std::string &curr_op = instr.op;
        if (curr_op.length() > 3 && curr_op.substr(0, 3) == "(f)" && curr_op.back() != '=')
        {
            if (is_num_constant(instr.arg1.value) && is_num_constant(instr.arg2.value))
            {
                bool modify_instr = true;
                char actual_op = curr_op.back();
                float arg1 = std::stof(instr.arg1.value);
                float arg2 = std::stof(instr.arg2.value);
                if (actual_op == '+')
                    arg1 = arg1 + arg2;
                else if (actual_op == '-')
                    arg1 = arg1 - arg2;
                else if (actual_op == '*')
                    arg1 = arg1 * arg2;
                else if (actual_op == '/')
                {
                    if (fabs(arg2) < 1e-9)
                        modify_instr = false;
                    if (modify_instr)
                        arg1 = arg1 / arg2;
                }
                if (std::isnan(arg1) || std::isinf(arg1))
                    modify_instr = false;
                if (modify_instr)
                {
                    curr_op = "=";
                    instr.arg1.value = std::to_string(arg1); // TODO : check
                    instr.arg2 = operand();                  // TODO : check
                }
            }
        }
        else if (curr_op == "+" || curr_op == "-" || curr_op == "*" || curr_op == "/" || curr_op == "%")
        {
            if (is_int_constant(instr.arg1.value) && is_int_constant(instr.arg2.value))
            {
                bool modify_instr = true;
                char actual_op = curr_op.back();
                int arg1 = std::stoi(instr.arg1.value);
                int arg2 = std::stoi(instr.arg2.value);
                if (actual_op == '+')
                    arg1 = arg1 + arg2;
                else if (actual_op == '-')
                    arg1 = arg1 - arg2;
                else if (actual_op == '*')
                    arg1 = arg1 * arg2;
                else if (actual_op == '/')
                {
                    if (arg2 == 0)
                        modify_instr = false;
                    if (modify_instr)
                        arg1 = arg1 / arg2;
                }
                else if (actual_op == '%')
                {
                    if (arg2 == 0)
                        modify_instr = false;
                    if (modify_instr)
                        arg1 = arg1 % arg2;
                }
                if (std::isnan(arg1) || std::isinf(arg1))
                    modify_instr = false;
                if (modify_instr)
                {
                    curr_op = "=";
                    instr.arg1.value = std::to_string(arg1);
                    instr.arg2 = operand(); // TODO : check
                }
            }
        }
        else if (curr_op.substr(0, 2) == "++" || curr_op.substr(0, 2) == "--" || curr_op == "!" || curr_op == "~" || curr_op == "unary-" || curr_op == "unary+")
        {
            if (is_int_constant(instr.arg1.value))
            {
                int arg1 = std::stoi(instr.arg1.value);
                if (curr_op.substr(0, 2) == "++")
                    arg1++;
                else if (curr_op.substr(0, 2) == "--")
                    arg1--;
                else if (curr_op == "!")
                    arg1 = !arg1;
                else if (curr_op == "~")
                    arg1 = ~arg1;
                else if (curr_op == "unary-")
                    arg1 = -arg1;
                else if (curr_op == "unary+")
                    arg1 = +arg1;
                curr_op = "=";
                instr.arg1.value = std::to_string(arg1);
            }
        }
        else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^")
        {
            if (is_int_constant(instr.arg1.value) && is_int_constant(instr.arg2.value))
            {
                int arg1 = std::stoi(instr.arg1.value);
                int arg2 = std::stoi(instr.arg2.value);
                if (curr_op == "==")
                    arg1 = arg1 == arg2;
                else if (curr_op == "!=")
                    arg1 = arg1 != arg2;
                else if (curr_op == "<")
                    arg1 = arg1 < arg2;
                else if (curr_op == ">")
                    arg1 = arg1 > arg2;
                else if (curr_op == "<=")
                    arg1 = arg1 <= arg2;
                else if (curr_op == ">=")
                    arg1 = arg1 >= arg2;
                else if (curr_op == "&&")
                    arg1 = arg1 && arg2;
                else if (curr_op == "||")
                    arg1 = arg1 || arg2;
                else if (curr_op == ">>")
                    arg1 = arg1 >> arg2;
                else if (curr_op == "<<")
                    arg1 = arg1 << arg2;
                else if (curr_op == "&")
                    arg1 = arg1 & arg2;
                else if (curr_op == "|")
                    arg1 = arg1 | arg2;
                else if (curr_op == "^")
                    arg1 = arg1 ^ arg2;
                curr_op = "=";
                instr.arg1.value = std::to_string(arg1);
                instr.arg2 = operand(); // TODO : check
            }
        }
        // computing conditional jumps
        else if (curr_op == "GOTO" && instr.arg1.value == "IF")
        {
            if (is_int_constant(instr.arg2.value))
            {
                if (std::stoi(instr.arg2.value) == 0)
                    add_instr = false;
                else
                {
                    instr.arg1 = operand(); // TODO : check
                    instr.arg2 = operand(); // TODO : check
                }
            }
        }

        if (add_instr)
        {
            int newid = folded_tac_code.size();
            id_map[instr.Label] = newid;
            instr.Label = newid;
            folded_tac_code.push_back(std::move(instr));
        }
    }
    for (auto &q : folded_tac_code)
    {
        if (q.gotoLabel != -1)
            q.gotoLabel = id_map[q.gotoLabel];
    }
    tac_code = std::move(folded_tac_code);
}

void dead_code_elimination()
{
    reconstruct_basic_blocks();
    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // reachability analysis (DFS)
    for (auto &func : function_cfgs)
    {
        std::vector<bool> visited(func.blocks.size(), false);
        if (func.blocks.empty())
            continue;

        visited[0] = true;
        std::stack<int> st;
        st.push(0);

        if (!func.adj.empty())
        {
            while (!st.empty())
            {
                int curr = st.top();
                st.pop();
                for (auto &next : func.adj[curr])
                {
                    if (!visited[next])
                    {
                        visited[next] = true;
                        st.push(next);
                    }
                }
            }
        }

        // Remove unreachable blocks in this function
        std::vector<std::vector<quad>> new_blocks;
        for (int i = 0; i < func.blocks.size(); i++)
        {
            if (visited[i])
                new_blocks.push_back(std::move(func.blocks[i]));
        }
        func.blocks = std::move(new_blocks);
    }

    reconstruct_basic_blocks();
    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // remove redundant jumps
    for (auto &func : function_cfgs)
    {
        for (auto i = 0; i < func.blocks.size() - 1; i++)
        {
            auto &block = func.blocks[i];
            if (block.empty())
                continue;

            const std::string &last_op = block.back().op;
            if (last_op == "GOTO")
            {
                bool keep_jump = false;
                for (auto &next : func.adj[i])
                {
                    if (next != i + 1)
                    {
                        keep_jump = true;
                        break;
                    }
                }
                if (!keep_jump)
                    block.pop_back();
            }
        }
    }

    reconstruct_basic_blocks();
    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // remove redundant labels
    for (auto &func : function_cfgs)
    {
        for (auto i = 1; i < func.blocks.size(); i++)
        {
            const auto &block = func.blocks[i];
            if (block.empty() || block[0].op.back() != ':' || block[0].op.substr(0, 5) == "FUNC_")
                continue;

            bool keep_label = false;
            for (auto &prev : func.rev_adj[i])
            {
                if (prev != i - 1)
                {
                    keep_label = true;
                    break;
                }
            }

            if (i > 0 && !func.blocks[i - 1].empty() && func.blocks[i - 1].back().op == "GOTO" && func.blocks[i - 1].back().gotoLabel == block[0].Label)
                keep_label = true;

            if (!keep_label)
            {
                std::vector<quad> new_block; // add all except first (label)
                for (auto j = 1; j < block.size(); j++)
                    new_block.push_back(std::move(block[j]));
                func.blocks[i] = std::move(new_block);
            }
        }
    }
    reconstruct_basic_blocks();
    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
}

// TO FIX : reaching copies analysis
// void transfer(const std::vector<quad> &block, std::unordered_set<std::pair<std::string, std::string>> initial_copies)
// {
//     auto copies = initial_copies;
//     for (const auto &instr : block)
//     {
//         if (instr.op == "=")
//         {
//             if (copies.find(std::make_pair(instr.arg1, instr.result)) != copies.end())
//                 continue;
//             std::string dst = instr.result;
//             auto it = copies.begin();
//             while (it != copies.end())
//             {
//                 const auto &[res, src] = *it;
//                 if (res == dst || src == dst)
//                     it = copies.erase(it);
//                 else
//                     ++it;
//             }
//             copies.insert(std::make_pair(instr.result, instr.arg1));
//         }
//     }
// }

void run_optimisations()
{
    if (tac_code.empty())
        return;
    constant_folding();
    dead_code_elimination();
    print_basic_blocks();
    print_separate_functions();
    print_cfg();
}
