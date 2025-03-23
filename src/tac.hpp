#ifndef TAC_HPP
#define TAC_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "data_structures.hpp"

class quad
{
public:
    int Label;
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
    int gotoLabel = -1;

    quad(int Label, std::string op, std::string arg1, std::string arg2, std::string result, int gotoLabel) : Label(Label), op(op), arg1(arg1), arg2(arg2), result(result), gotoLabel(gotoLabel) {}
};

// extern std::vector<quad> tac_code;

void run_optimisations();

std::string getTempVariable(std::string);
void print_tac_code(const std::string &inputFile);
int getCurrentSize();
std::vector<int> mergeList(std::vector<int> &list1, std::vector<int> &list2);
int emit(std::string, std::string, std::string, std::string, int);
void backpatch(std::vector<int> &, int);
void casepatch(std::vector<int> &bplist, std::string target);
void remainingBackpatch();

void setArg1(int ind, std::string value);
void setResult(int ind, std::string value);
int assign_exp(std::string op, std::string type, std::string type1, std::string type2, std::string arg1, std::string arg2);

extern std::vector<quad> tac_code;
extern std::vector<std::vector<quad>> basic_blocks;

void generate_ir();

void compute_basic_blocks();
void print_basic_blocks();

inline std::string stringify(const quad &instr)
{
    std::string s = std::to_string(instr.Label) + ":  ";
    const std::string &curr_op = instr.op;
    const char fi = curr_op[0];

    if (curr_op.back() == ':' || curr_op.substr(0, 5) == "FUNC_")
        s += curr_op;
    else if (curr_op.substr(0, 2) == "++" || curr_op.substr(1, 2) == "++" || curr_op.substr(0, 2) == "--" || curr_op.substr(1, 2) == "--" || curr_op == "!" || curr_op == "~" || curr_op == "unary-" || curr_op == "unary+" || curr_op == "unary&" || curr_op == "unary*" || curr_op == "SIZEOF")
        s += instr.result + " = " + curr_op + " " + instr.arg1;
    else if (curr_op == "=")
        s += instr.result + " = " + instr.arg1;
    else if (fi == '+' || fi == '-' || fi == '*' || fi == '/' || fi == '%')
        s += instr.result + " = " + instr.arg1 + " " + fi + " " + instr.arg2 + "\t(" + curr_op.substr(1) + ")";
    else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^" || curr_op == "PTR_OP" || curr_op == "member_access")
        s += instr.result + " = " + instr.arg1 + " " + curr_op + " " + instr.arg2;
    else if (curr_op == "RETURN" || curr_op == "param")
        s += curr_op + " " + instr.arg1;
    else if (curr_op == "CALL")
        s += instr.result + " = " + curr_op + " " + instr.arg1 + " " + instr.arg2;
    else if (curr_op == "[ ]")
        s += instr.result + " = " + instr.arg1 + "[" + instr.arg2 + "]";
    else if (curr_op == "GOTO")
    {
        if (instr.arg1 == "IF")
            s += "IF " + instr.arg2 + " GOTO " + std::to_string(instr.gotoLabel);
        else
            s += "GOTO " + std::to_string(instr.gotoLabel);
    }
    return s;
}

#endif
