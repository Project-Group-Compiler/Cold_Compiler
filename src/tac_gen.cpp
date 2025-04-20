#include <fstream>
#include <iostream>
#include <map>
#include "tac.hpp"
#include "data_structures.hpp"
#include "types.hpp"

extern std::string outputDir;
std::vector<quad> tac_code;
long long counter = 0;
void print_error(const std::string &message);
std::vector<quad> staticAddLater;
std::vector<operand> static_vars;

int getCurrentSize()
{
    return tac_code.size();
}

void setArg1(int ind, operand value)
{
    tac_code[ind].arg1 = value;
}

void setResult(int ind, operand value)
{
    tac_code[ind].result = value;
}

void extendList(std::vector<int> &list1, std::vector<int> &list2)
{
    for (auto &it : list2)
        list1.push_back(it);
}

int emit(std::string op, operand arg1, operand arg2, operand result, int gotoLabel)
{
    tac_code.emplace_back(tac_code.size(), op, arg1, arg2, result, gotoLabel);
    return (int)tac_code.size() - 1;
}

void backpatch(std::vector<int> &nextList, int gotoLabel)
{
    for (auto i : nextList)
    {
        tac_code[i].gotoLabel = gotoLabel;
    }
}

void singlePatch(int label, int gotoLabel)
{
    tac_code[label].gotoLabel = gotoLabel;
}

void casepatch(std::vector<int> &bplist, operand target)
{
    for (auto i : bplist)
    {
        tac_code[i].arg1 = target;
    }
}

operand getTempVariable(std::string type)
{
    std::string tempName = "_t_" + std::to_string(counter++);
    insertSymbol(*curr_table, tempName, type, getSize(type), 0, NULL); // TODO: Check
    operand q = {tempName, lookup(tempName)};
    return q;
}

// Emit for assignment expressions
// int assign_exp(std::string op, std::string type, std::string type1, std::string type2, operand arg1, operand arg2, int isLocalStaticInit)
// {
//     // std::cerr << "assign_exp: " << op << "#" << type << "#" << type1 << "#" << type2 << "#" << arg1.value << "#" << arg2.value << std::endl;
//     if (isLocalStaticInit > 0)
//         arg1.value += "_" + std::to_string(isLocalStaticInit);
//     std::string temp_op = "";
//     std::string str = op;
//     operand q;

//     str.pop_back();
//     /*
//     if(($1->type).back() == '*' && (($3->type == "int") || ($3->type == "Integer Constant"))){  //int** + ...
//                 operand q2 = getTempVariable($3->type);
//                 emit("*", $3->place, {std::to_string(getSize($1->type.substr(0, $1->type.size()-1)))}, q2, -1);
//                 emit("ptr-", $1->place, q2, q, -1);
//             }else{
//     */
//     if (op != "=")
//     {
//         temp_op = str;
//         if (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%=")
//         {
//             if ((op == "+=" || op == "-=") && (type1.back() == '*' && (type2 == "int" || type2 == "Integer Constant")))
//             {
//                 operand q1 = getTempVariable(type2);
//                 emit("*", arg2, {std::to_string(getSize(type1.substr(0, type1.size() - 1)))}, q1, -1);
//                 q = getTempVariable(type);
//                 emit("ptr" + temp_op, arg1, q1, q, -1);
//             }
//             else
//             {
//                 if (isFloat(type1) && checkInt(type2))
//                 {
//                     operand q1 = getTempVariable(type1);
//                     emit("intToFloat", arg2, {}, q1, -1);
//                     q = getTempVariable(type);
//                     emit("(f)" + temp_op, arg1, q1, q, -1);
//                 }
//                 else if (checkInt(type1) && isFloat(type2))
//                 {
//                     operand q1 = getTempVariable(type1);
//                     emit("floatToInt", arg2, {}, q1, -1);
//                     q = getTempVariable(type);
//                     emit(temp_op, arg1, q1, q, -1);
//                 }
//                 else if (isFloat(type1) && isFloat(type2))
//                 {
//                     q = getTempVariable(type);
//                     emit("(f)" + temp_op, arg1, arg2, q, -1);
//                 }
//                 else
//                 {
//                     q = getTempVariable(type);
//                     emit(temp_op, arg1, arg2, q, -1);
//                 }
//             }
//         }
//         else
//         {
//             q = getTempVariable(type);
//             emit(temp_op, arg1, arg2, q, -1);
//         }
//     }
//     else
//     {
//         if (isFloat(type1) && checkInt(type2))
//         {
//             q = getTempVariable(type1);
//             emit("intToFloat", arg2, operand(), q, -1);
//         }
//         else if (checkInt(type1) && isFloat(type2))
//         {
//             q = getTempVariable(type1);
//             emit("floatToInt", arg2, operand(), q, -1);
//         }
//         else
//             q = arg2;
//     }
//     int x;
//     if (isFloat(type))
//     {
//         if (isLocalStaticInit > 0)
//             staticAddLater.push_back(quad(staticAddLater.size(), "(f)=", q, operand(), arg1, -1));
//         else
//             x = emit("(f)=", q, operand(), arg1, -1);
//     }
//     else
//     {
//         if (isLocalStaticInit > 0)
//             staticAddLater.push_back(quad(staticAddLater.size(), "=", q, operand(), arg1, -1));
//         else
//             x = emit("=", q, operand(), arg1, -1);
//     }
//     return x;
// }

int assign_exp(std::string op, std::string type, std::string type1, std::string type2, operand arg1, operand arg2, int isLocalStaticInit)
{
    // std::cerr << "assign_exp: " << op << "#" << type << "#" << type1 << "#" << type2 << "#" << arg1.value << "#" << arg2.value << std::endl;
    if (isLocalStaticInit > 0)
        arg1.value += "_" + std::to_string(isLocalStaticInit);
    std::string temp_op = "";
    std::string str = op;
    operand q;

    str.pop_back();
    /*
    if(($1->type).back() == '*' && (($3->type == "int") || ($3->type == "Integer Constant"))){  //int** + ...
                operand q2 = getTempVariable($3->type);
                emit("*", $3->place, {std::to_string(getSize($1->type.substr(0, $1->type.size()-1)))}, q2, -1);
                emit("ptr-", $1->place, q2, q, -1);
            }else{
    */
    if (op != "=")
    {
        temp_op = str;
        if (op == "+=" || op == "-=" || op == "*=" || op == "/=" || op == "%=")
        {
            if ((op == "+=" || op == "-=") && (type1.back() == '*' && (type2 == "int" || type2 == "Integer Constant")))
            {
                operand q1 = getTempVariable(type2);
                emit("*", arg2, {std::to_string(getSize(type1.substr(0, type1.size() - 1)))}, q1, -1);
                q = getTempVariable(type);
                emit("ptr" + temp_op, arg1, q1, q, -1);
            }
            else
            {
                if (isFloat(type1) && checkInt(type2))
                {
                    operand q1 = getTempVariable(type1);
                    emit("intToFloat", arg2, {}, q1, -1);
                    q = getTempVariable(type);
                    emit("(f)" + temp_op, arg1, q1, q, -1);
                }
                else if (checkInt(type1) && isFloat(type2))
                {
                    operand q1 = getTempVariable(type1);
                    emit("floatToInt", arg2, {}, q1, -1);
                    q = getTempVariable(type);
                    emit(temp_op, arg1, q1, q, -1);
                }
                else if (isChar(type1) && checkInt(type2))
                {
                    operand q1 = getTempVariable(type1);
                    emit("intToChar", arg2, {}, q1, -1);
                    q = getTempVariable(type);
                    emit(temp_op, arg1, q1, q, -1);
                }
                else if (checkInt(type1) && isChar(type2))
                {
                    operand q1 = getTempVariable(type1);
                    emit("charToInt", arg2, {}, q1, -1);
                    q = getTempVariable(type);
                    emit(temp_op, arg1, q1, q, -1);
                }
                else if (isChar(type1) && isFloat(type2))
                {
                    operand q1 = getTempVariable("int");
                    emit("floatToInt", arg2, {}, q1, -1);
                    operand q2 = getTempVariable(type1);
                    emit("intToChar", q1, {}, q2, -1);
                    q = getTempVariable(type);
                    emit(temp_op, arg1, q2, q, -1);
                }
                else if (isFloat(type1) && isChar(type2))
                {
                    operand q1 = getTempVariable("int");
                    emit("charToInt", arg2, {}, q1, -1);
                    operand q2 = getTempVariable(type1);
                    emit("intToFloat", q1, {}, q2, -1);
                    q = getTempVariable(type);
                    emit("(f)" + temp_op, arg1, q1, q, -1);
                }
                else if (isFloat(type1) && isFloat(type2))
                {
                    q = getTempVariable(type);
                    emit("(f)" + temp_op, arg1, arg2, q, -1);
                }
                else
                {
                    q = getTempVariable(type);
                    emit(temp_op, arg1, arg2, q, -1);
                }
            }
        }
        else
        {
            q = getTempVariable(type);
            emit(temp_op, arg1, arg2, q, -1);
        }
    }
    else
    {
        if (isFloat(type1) && checkInt(type2))
        {
            q = getTempVariable(type1);
            emit("intToFloat", arg2, operand(), q, -1);
        }
        else if (checkInt(type1) && isFloat(type2))
        {
            q = getTempVariable(type1);
            emit("floatToInt", arg2, operand(), q, -1);
        }
        else if (isChar(type1) && checkInt(type2))
        {
            q = getTempVariable(type1);
            emit("intToChar", arg2, operand(), q, -1);
        }
        else if (checkInt(type1) && isChar(type2))
        {
            q = getTempVariable(type1);
            emit("charToInt", arg2, operand(), q, -1);
        }
        else if (isChar(type1) && isFloat(type2))
        {
            operand q2 = getTempVariable("int");
            emit("floatToInt", arg2, operand(), q2, -1);
            q = getTempVariable(type1);
            emit("intToChar", q2, operand(), q, -1);
        }
        else if (isFloat(type1) && isChar(type2))
        {
            operand q2 = getTempVariable("int");
            emit("charToInt", arg2, operand(), q2, -1);
            q = getTempVariable(type1);
            emit("intToFloat", q2, operand(), q, -1);
        }
        else
            q = arg2;
    }
    int x;
    if (isFloat(type))
    {
        if (isLocalStaticInit > 0)
            staticAddLater.push_back(quad(staticAddLater.size(), "(f)=", q, operand(), arg1, -1));
        else
            x = emit("(f)=", q, operand(), arg1, -1);
    }
    else if (isChar(type))
    {
        if (isLocalStaticInit > 0)
            staticAddLater.push_back(quad(staticAddLater.size(), "(c)=", q, operand(), arg1, -1));
        else
            x = emit("(c)=", q, operand(), arg1, -1);
    }
    else
    {
        if (isLocalStaticInit > 0)
            staticAddLater.push_back(quad(staticAddLater.size(), "=", q, operand(), arg1, -1));
        else
            x = emit("=", q, operand(), arg1, -1);
    }
    return x;
}

void remainingBackpatch()
{
    int i, j;
    i = j = (int)tac_code.size() - 1;
    i--;
    while (tac_code[i].op.substr(0, 5) != "FUNC_")
    {
        if (tac_code[i].op == "GOTO" && tac_code[i].gotoLabel == 0)
            tac_code[i].gotoLabel = j;
        i--;
    }
}

void addStaticInit()
{
    for (auto &instr : tac_code)
    {
        instr.Label += staticAddLater.size();
        if (instr.op == "GOTO" && instr.gotoLabel != -1)
            instr.gotoLabel += staticAddLater.size();
    }
    for (auto &instr : tac_code)
        staticAddLater.push_back(instr);
    tac_code = staticAddLater;
    staticAddLater.clear();
}

void addgotoLabels()
{
    std::map<int, std::vector<int>> gotoTargets;
    for (auto &q : tac_code)
    {
        if (q.op == "GOTO" && q.gotoLabel != -1)
            gotoTargets[q.gotoLabel].push_back(q.Label);
    }
    int labelcnt = 0;
    std::vector<int> id_map(tac_code.size(), -1);
    std::vector<quad> labelled_tac_code;
    for (auto &q : tac_code)
    {
        int newid = labelled_tac_code.size();
        if (gotoTargets.find(q.Label) != gotoTargets.end())
        {
            labelled_tac_code.push_back(quad(newid, ("L" + std::to_string(labelcnt) + " :"), operand(), operand(), operand(), -1));
            newid++;
            labelcnt++;
        }
        id_map[q.Label] = newid;
        q.Label = newid;
        labelled_tac_code.push_back(std::move(q));
    }
    for (auto &[a, v] : gotoTargets)
        for (auto &x : v)
            labelled_tac_code[id_map[x]].gotoLabel = id_map[a] - 1;
    tac_code = std::move(labelled_tac_code);
}

void print_tac_code(const std::string &inputFile, bool modifygotoLabels)
{
    std::ofstream out(outputDir + inputFile + "_IR.txt");
    if (!out)
    {
        print_error("cannot open " + outputDir + inputFile + "_IR.txt");
        return;
    }

    out << "Three Address Code:\n\n";
    for (auto &instr : tac_code)
        out << stringify(instr, modifygotoLabels) << "\n";
    out << "\n"
        << std::string(60, '-') << "\n";

    std::ofstream tout(outputDir + inputFile + "_TABLE_IR.txt");
    if (!tout)
    {
        print_error("cannot open " + outputDir + inputFile + "_TABLE_IR.txt");
        return;
    }
    tout << std::left << std::setw(10) << "Label" << std::setw(50) << "Op" << std::setw(20) << "Arg1"
         << std::setw(20) << "Arg2" << std::setw(20) << "Result" << std::setw(20) << "Goto" << "\n";
    tout << std::string(150, '-') << "\n";

    for (size_t i = 0; i < tac_code.size(); i++)
    {
        const auto &q = tac_code[i];
        std::string gotolabel = (q.gotoLabel < 0) ? "" : std::to_string(q.gotoLabel);
        if (modifygotoLabels && q.gotoLabel >= 0)
            gotolabel = "L" + std::to_string(q.gotoLabel);
        tout << std::left << std::setw(10) << q.Label << std::setw(50) << q.op
             << std::setw(20) << q.arg1.value << std::setw(20) << q.arg2.value
             << std::setw(20) << q.result.value
             << std::setw(20) << gotolabel
             << "\n";
    }
}

void generate_ir()
{
    addStaticInit();
}
