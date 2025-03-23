#include "tac.hpp"
#include <unordered_map>
#include <stack>

std::vector<std::vector<quad>> basic_blocks;
std::unordered_map<int, int> leader_block_map;
std::vector<std::vector<int>> adj, rev_adj;

bool is_int_constant(const std::string &s)
{
    if (s.empty())
        return false;
    for (auto c : s)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

void compute_map()
{
    for (auto i = 0; i < basic_blocks.size(); i++)
        leader_block_map[basic_blocks[i][0].Label] = i;
}

void compute_basic_blocks()
{
    basic_blocks.clear();
    std::vector<quad> block;
    for (auto &instr : tac_code)
    {
        const std::string &curr_op = instr.op.first;
        if (curr_op.back() == ':')
        {
            if ((curr_op.substr(0, 5) == "FUNC_" && curr_op.substr(curr_op.length() - 3) == "end"))
            {
                block.push_back(instr);
                basic_blocks.push_back(block);
                block.clear();
            }
            else
            {
                if (!block.empty())
                {
                    basic_blocks.push_back(block);
                    block.clear();
                }
                block.push_back(instr);
            }
        }
        else if (curr_op == "GOTO")
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

    compute_map();
}

void print_basic_blocks()
{
    int block_no = 0;
    for (auto &block : basic_blocks)
    {
        std::cout << "Block " << block_no++ << ":\n";
        for (auto &instr : block)
        {
            std::cout << stringify(instr) << std::endl;
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

void build_cfg()
{
    if (basic_blocks.empty())
        return;
    adj.clear();
    rev_adj.clear();
    adj.resize(basic_blocks.size());
    rev_adj.resize(basic_blocks.size());
    for (auto i = 0; i < basic_blocks.size(); i++)
    {
        const auto &block = basic_blocks[i];
        if (block.empty())
            continue;
        const std::string &last_op = block.back().op.first;
        if (last_op == "GOTO")
        {
            int target_block = leader_block_map[block.back().gotoLabel];
            adj[i].push_back(target_block);
            rev_adj[target_block].push_back(i);
            if (block.back().arg1.first == "IF")
            {
                if (i + 1 < basic_blocks.size() && i + 1 != target_block)
                {
                    adj[i].push_back(i + 1);
                    rev_adj[i + 1].push_back(i);
                }
            }
        }
        else
        {
            if (i + 1 < basic_blocks.size())
            {
                adj[i].push_back(i + 1);
                rev_adj[i + 1].push_back(i);
            }
        }
    }
}

void print_cfg()
{
    std::cout << "Control Flow Graph:\n";
    std::cout << "-------------------\n";

    for (size_t i = 0; i < adj.size(); i++)
    {
        // Print basic block
        std::cout << "Block " << i << ":\n";
        for (const auto &instr : basic_blocks[i])
        {
            std::cout << "  " << stringify(instr) << "\n";
        }

        // Print outgoing edges
        std::cout << "Successors: ";
        if (adj[i].empty())
        {
            std::cout << "none";
        }
        else
        {
            for (size_t j = 0; j < adj[i].size(); j++)
            {
                std::cout << adj[i][j];
                if (j < adj[i].size() - 1)
                    std::cout << ", ";
            }
        }
        std::cout << "\n";

        // Print incoming edges
        std::cout << "Predecessors: ";
        if (rev_adj[i].empty())
        {
            std::cout << "none";
        }
        else
        {
            for (size_t j = 0; j < rev_adj[i].size(); j++)
            {
                std::cout << rev_adj[i][j];
                if (j < rev_adj[i].size() - 1)
                    std::cout << ", ";
            }
        }
        std::cout << "\n\n";
    }
}

void constant_folding()
{
    std::vector<int> id_map(tac_code.size(), -1);
    std::vector<quad> folded_tac_code;
    for (quad &instr : tac_code)
    {
        bool add_instr = true;
        std::string &curr_op = instr.op.first;
        // if (curr_op.length() > 3 && curr_op.substr(1) == "int")
        if (curr_op == "+" || curr_op == "-" || curr_op == "*" || curr_op == "/" || curr_op == "%")
        {
            if (is_int_constant(instr.arg1.first) && is_int_constant(instr.arg2.first))
            {
                int arg1 = std::stoi(instr.arg1.first);
                int arg2 = std::stoi(instr.arg2.first);
                if (curr_op[0] == '+')
                    arg1 = arg1 + arg2;
                else if (curr_op[0] == '-')
                    arg1 = arg1 - arg2;
                else if (curr_op[0] == '*')
                    arg1 = arg1 * arg2;
                else if (curr_op[0] == '/')
                    arg1 = arg1 / arg2;
                else if (curr_op[0] == '%')
                    arg1 = arg1 % arg2;
                curr_op = "=";
                instr.arg1.first = std::to_string(arg1);
                instr.arg2.first = "";
            }
        }
        else if (curr_op.substr(0, 2) == "++" || curr_op.substr(0, 2) == "--" || curr_op == "!" || curr_op == "~" || curr_op == "unary-" || curr_op == "unary+")
        {
            if (is_int_constant(instr.arg1.first))
            {
                int arg1 = std::stoi(instr.arg1.first);
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
                instr.arg1.first = std::to_string(arg1);
            }
        }
        else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^")
        {
            if (is_int_constant(instr.arg1.first) && is_int_constant(instr.arg2.first))
            {
                int arg1 = std::stoi(instr.arg1.first);
                int arg2 = std::stoi(instr.arg2.first);
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
                instr.arg1.first = std::to_string(arg1);
                instr.arg2.first = "";
            }
        }
        // computing conditional jumps
        else if (curr_op == "GOTO" && instr.arg1.first == "IF")
        {
            if (is_int_constant(instr.arg2.first))
            {
                if (std::stoi(instr.arg2.first) == 0)
                    add_instr = false;
                else
                {
                    instr.arg1.first = "";
                    instr.arg2.first = "";
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
    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // reachability analysis (DFS)
    std::vector<bool> visited(basic_blocks.size(), false);
    visited[0] = true;
    std::stack<int> st;
    st.push(0);
    while (!st.empty())
    {
        int curr = st.top();
        st.pop();
        for (auto &next : adj[curr])
        {
            if (!visited[next])
            {
                visited[next] = true;
                st.push(next);
            }
        }
    }
    // remove unreachable blocks
    std::vector<std::vector<quad>> new_blocks;
    for (int i = 0; i < basic_blocks.size(); i++)
    {
        if (visited[i])
            new_blocks.push_back(std::move(basic_blocks[i]));
    }
    basic_blocks = std::move(new_blocks);

    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // remove redundant jumps
    for (auto i = 0; i < basic_blocks.size() - 1; i++)
    {
        auto &block = basic_blocks[i];
        const std::string &last_op = block.back().op.first;
        if (last_op == "GOTO")
        {
            bool keep_jump = false;
            for (auto &x : adj[i])
            {
                if (x != i + 1)
                {
                    keep_jump = true;
                    break;
                }
            }
            if (!keep_jump)
                block.pop_back();
        }
    }

    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
    // remove redundant labels
    for (auto i = 1; i < basic_blocks.size(); i++)
    {
        const auto &block = basic_blocks[i];
        if (block[0].op.first.back() == ':' && block[0].op.first.substr(0, 5) != "FUNC_")
        {
            bool keep_label = false;
            for (auto &x : rev_adj[i])
            {
                if (x != i - 1)
                {
                    keep_label = true;
                    break;
                }
            }
            if (!keep_label)
            {
                std::vector<quad> new_block; // add all except first (label)
                for (auto j = 1; j < block.size(); j++)
                    new_block.push_back(std::move(block[j]));
                basic_blocks[i] = std::move(new_block);
            }
        }
    }

    blocks_to_code();
    compute_basic_blocks();
    build_cfg();
}

void run_optimisations()
{
    constant_folding();
    dead_code_elimination();
    for (auto &instr : tac_code)
        std::cout << stringify(instr) << "\n";
    std::cout << "\n-------------------------\n\n";
}