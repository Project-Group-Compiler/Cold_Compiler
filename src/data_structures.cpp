#include "data_structures.hpp"
#include <fstream>
#include <iomanip>
#include <vector>

std::vector<SymbolTableEntry> SymbolTable;
std::vector<SymbolTableEntry> ConstantTable;
std::vector<std::pair<std::string, std::string>> typedefTable;
bool onlyLexin = false;

void addToSymbolTable(std::string Token, std::string Data_type)
{
    SymbolTableEntry entry;
    entry.token = Token;
    entry.data_type = Data_type;
    SymbolTable.push_back(entry);
}

void addStandardProceduresToSymbolTable()
{
    std::vector<std::pair<std::string, std::string>> procedures = {
        {"printf", "Procedure"},
        {"scanf", "Procedure"},
        {"malloc", "Procedure"},
        {"calloc", "Procedure"},
        {"free", "Procedure"},
        {"fopen", "Procedure"},
        {"fputs", "Procedure"},
        {"fgets", "Procedure"},
        {"fclose", "Procedure"},
        {"fprintf", "Procedure"},
        {"fscanf", "Procedure"},
        {"fputc", "Procedure"},
        {"fgetc", "Procedure"},
        {"strlen", "Procedure"},
        {"strcmp", "Procedure"},
        {"strncmp", "Procedure"},
        {"strcpy", "Procedure"},
        {"strcat", "Procedure"},
        {"strncat", "Procedure"},
        {"atoi", "Procedure"}};

    addToSymbolTable("     ", "     ");
    for (const auto &procedure : procedures)
    {
        addToSymbolTable(procedure.first, procedure.second);
    }
}

void updateLastSymbolEntry()
{
    if (!SymbolTable.empty())
    {
        SymbolTable.back().data_type += "[]";
    }
    else
    {
        std::cerr << "SymbolTable is already empty." << std::endl;
    }
}

void updateFuncSymbolEntry(int args)
{
    if (SymbolTable.size() > args)
    {
        SymbolTable[SymbolTable.size() - 1 - args].data_type = "Procedure";
    }
    else
    {
        std::cerr << "SymbolTable is too small" << std::endl;
    }
}

void printSymbolTable_meet(std::ofstream &out)
{
    out << "\nSymbol Table:\n\n";
    out << std::left << std::setw(60) << "Token"
        << std::setw(25) << "Data Type" << std::endl;
    out << std::string(90, '-') << "\n";
    for (int i = 0; i < SymbolTable.size(); i++)
    {
        out << std::left << std::setw(60) << SymbolTable[i].token
            << std::setw(25) << SymbolTable[i].data_type << "\n";
    }
    out << std::string(90, '-') << "\n";
}

void addToConstantTable(std::string constnt, std::string const_type)
{
    SymbolTableEntry entry;
    entry.token = constnt;
    entry.data_type = const_type;
    ConstantTable.push_back(entry);
}

void printConstantTable(std::ofstream &out)
{
    if (ConstantTable.empty())
        return;
    out << "\nConstant Table:\n\n";
    out << std::left << std::setw(60) << "Constant"
        << std::setw(25) << "Constant Type" << std::endl;
    out << std::string(90, '-') << "\n";
    for (int i = 0; i < ConstantTable.size(); i++)
    {
        out << std::left << std::setw(60) << ConstantTable[i].token
            << std::setw(25) << ConstantTable[i].data_type << "\n";
    }
    out << std::string(90, '-') << "\n";
}

std::string searchTypedefTable(std::string Token)
{
    for (int i = 0; i < typedefTable.size(); i++)
    {
        if (typedefTable[i].first == Token)
            return typedefTable[i].second;
    }
    return "";
}

void printType(std::ofstream &out)
{
    if (typedefTable.empty())
        return;
    out << "\nTypedef Table:\n\n";
    out << std::left << std::setw(60) << "Definition"
        << std::setw(25) << "Keyword" << std::endl;
    out << std::string(90, '-') << "\n";
    for (int i = 0; i < typedefTable.size(); i++)
    {
        out << std::left << std::setw(60) << typedefTable[i].first
            << std::setw(25) << typedefTable[i].second << "\n";
    }
    out << std::string(90, '-') << "\n";
}

extern std::string outputDir;
void print_error(const std::string &message);

void printTables(const std::string &inputFile)
{
    addStandardProceduresToSymbolTable();
    std::ofstream out(outputDir + inputFile + "_tables.txt");
    if (!out)
    {
        print_error("cannot open " + outputDir + inputFile + "_tables.txt");
        return;
    }
    printSymbolTable_meet(out);
    printConstantTable(out);
    printType(out);
}

std::string getSizeOfType(const std::string& typeStr) {
    if (typeStr == "int") {
        return "4";
    } else if (typeStr == "int*") {
        return "4";
    } else if(typeStr.size() >= 4 && typeStr.substr(0, 4) == "int*"){//for int****
        return "4";
    } else {
        return "0"; // Unknown type
    }
}