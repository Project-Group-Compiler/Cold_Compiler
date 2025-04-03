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

void run_optimisations();

std::string getTempVariable(std::string);
void print_tac_code(const std::string &inputFile);
int getCurrentSize();
void extendList(std::vector<int> &list1, std::vector<int> &list2);
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
    else if (curr_op.substr(0, 2) == "++" || curr_op.substr(1, 2) == "++" || curr_op.substr(0, 2) == "--" || curr_op.substr(1, 2) == "--" || curr_op == "!" || curr_op == "~")
        s += instr.result + " = " + curr_op + " " + instr.arg1;
    else if (curr_op.substr(0, 5) == "unary")
        s += instr.result + " = " + curr_op.back() + " " + instr.arg1;
    else if (curr_op.length() == 4 && curr_op[0] == '(')
    {
        if (curr_op.back() == '=')
            s += instr.result + " = " + instr.arg1 + "\t\t(";
        else
            s += instr.result + " = " + instr.arg1 + " " + curr_op.back() + " " + instr.arg2 + "\t\t(";
        if (curr_op[1] == 'f')
            s += "float)";
        else if (curr_op[1] == 'i')
            s += "int)";
    }
    else if (curr_op == "=")
        s += instr.result + " = " + instr.arg1;
    else if (curr_op == "+" || curr_op == "-" || curr_op == "*" || curr_op == "/" || curr_op == "%")
        s += instr.result + " = " + instr.arg1 + " " + curr_op + " " + instr.arg2;
    else if (curr_op == "==" || curr_op == "!=" || curr_op == "<" || curr_op == ">" || curr_op == "<=" || curr_op == ">=" || curr_op == "&&" || curr_op == "||" || curr_op == ">>" || curr_op == "<<" || curr_op == "&" || curr_op == "|" || curr_op == "^" || curr_op == "ptr+")
        s += instr.result + " = " + instr.arg1 + " " + curr_op + " " + instr.arg2;
    else if (curr_op == "RETURN" || curr_op == "param")
        s += curr_op + " " + instr.arg1;
    else if (curr_op == "CALL")
    {
        s += curr_op + " " + instr.arg1 + " " + instr.arg2;
        auto pos = s.find(":  ");
        if (instr.result != "")
            s = s.substr(0, pos + 3) + instr.result + " = " + s.substr(pos + 3);
    }
    else if (curr_op == "[ ]")
        s += instr.result + " = " + instr.arg1 + "[" + instr.arg2 + "]";
    else if (curr_op == "GOTO")
    {
        if (instr.arg1 == "IF")
            s += "IF " + instr.arg2 + " GOTO " + std::to_string(instr.gotoLabel);
        else
            s += "GOTO " + std::to_string(instr.gotoLabel);
    }
    else if (curr_op == "CopyToOffset")
        s += "Copy " + instr.arg1 + " to offset " + instr.arg2 + " of " + instr.result;
    else if (curr_op == "intToFloat" || curr_op == "SIZEOF")
        s += instr.result + " = " + curr_op + "(" + instr.arg1 + ")";
    else if (curr_op.substr(0, 5) == "CAST_")
        s += instr.result + " = CAST " + instr.arg1 + " to " + curr_op.substr(5);
    return s;
}

#endif
