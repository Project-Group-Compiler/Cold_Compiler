#include <fstream>
#include <iostream>
#include <map>
#include "tac.hpp"

extern std::string outputDir;
std::vector<quad> tac_code;
long long counter = 0;
void print_error(const std::string &message);

int getCurrentSize()
{
    return tac_code.size();
}

void setArg1(int ind, qid value)
{
    tac_code[ind].arg1 = value;
}

void setResult(int ind, qid value)
{
    tac_code[ind].result = value;
}

std::vector<int> mergeList(std::vector<int> &list1, std::vector<int> &list2)
{
    std::vector<int> mrgList = list1;
    for (auto &it : list2)
        mrgList.push_back(it);
    return mrgList;
}

int emit(qid op, qid arg1, qid arg2, qid result, int gotoLabel)
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

void casepatch(std::vector<int> &bplist, qid target)
{
    for (auto i : bplist)
    {
        tac_code[i].arg1 = target;
    }
}

qid getTempVariable(std::string type)
{
    std::string tempName = "_t_" + std::to_string(counter++);

    addToSymbolTable(tempName, type);
    SymbolTableEntry *entry = new SymbolTableEntry;
    entry->token = tempName;
    entry->data_type = type;

    return qid(tempName, entry);
}

// emitting code for all type of assignment expressions
// TODO: Handle float
int assign_exp(std::string op, std::string type, std::string type1, std::string type2, qid arg1, qid arg2)
{
    std::string temp_op = "";
    std::string str = op;

    qid q;

    str.pop_back();
    if (op != "=")
    {
        temp_op = str;
        q = getTempVariable(type);
    }
    else
    {
        q = arg2;
    }

    if (op != "=")
    {
        emit(qid(temp_op, lookup(str)), arg1, arg2, q, -1);
    }

    int x = emit(qid("=", lookup("=")), q, qid("", NULL), arg1, -1);

    return x;
}

void remainingBackpatch()
{
    int i, j;
    i = j = (int)tac_code.size() - 1;
    i--;
    while (tac_code[i].op.first.substr(0, 5) != "FUNC_")
    {
        if (tac_code[i].op.first == "GOTO" && tac_code[i].gotoLabel == 0)
            tac_code[i].gotoLabel = j;
        i--;
    }
}

void addgotoLabels()
{
    std::map<int, std::vector<int>> gotoTargets;
    for (auto &q : tac_code)
    {
        if (q.op.first == "GOTO" && q.gotoLabel != -1)
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
            labelled_tac_code.push_back(quad(newid, qid("L" + std::to_string(labelcnt) + " :", NULL), qid("", nullptr), qid("", nullptr), qid("", nullptr), -1));
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

void print_tac_code(const std::string &inputFile)
{
    // std::ofstream out(outputDir + inputFile + "_IR.txt");
    // if (!out) {
    //     print_error("cannot open " + outputDir + inputFile + "_IR.txt");
    //     return;
    // }

    // out << "Three Address Code:\n\n";
    std::cout << std::setw(5) << "Label" << std::setw(20) << "Op" << std::setw(20) << "Arg1"
              << std::setw(20) << "Arg2" << std::setw(20) << "Result" << std::setw(20) << "Goto" << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------------------\n";

    for (size_t i = 0; i < tac_code.size(); i++)
    { // FUNC -2 is also suppressed
        const auto &q = tac_code[i];
        std::cout << std::setw(5) << q.Label << std::setw(20) << q.op.first
                  << std::setw(20) << q.arg1.first << std::setw(20) << q.arg2.first
                  << std::setw(20) << q.result.first
                  << std::setw(20) << (q.gotoLabel < 0 ? "" : std::to_string(q.gotoLabel))
                  << "\n";
    }

    // for (auto &instr : tac_code)
    // {
    //     std::cout << stringify(instr) << std::endl;
    // }
    // std::cout << "\n-------------------------\n\n";
    // addgotoLabels();
}

void generate_ir()
{
    for (auto &instr : tac_code)
        std::cout << stringify(instr) << "\n";
    std::cout << "\n-------------------------\n\n";
    addgotoLabels();
}
