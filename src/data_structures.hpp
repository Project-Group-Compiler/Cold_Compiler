#pragma once
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstring>
#include <fstream>

struct TOKEN
{
    int line;
    int column;
    std::string token_type;
    std::string lexeme;
};

typedef struct
{
    std::string token;
    std::string data_type;
} SymbolTableEntry;

extern std::vector<SymbolTableEntry> SymbolTable;
extern std::vector<SymbolTableEntry> ConstantTable;
extern std::vector<std::pair<std::string, std::string>> typedefTable;
extern bool onlyLexin;

void addToSymbolTable(std::string Token, std::string Data_type);
void addStandardProceduresToSymbolTable();
void updateLastSymbolEntry();
void updateFuncSymbolEntry(int args);
void printSymbolTable(std::ofstream &out);

void addToConstantTable(std::string constnt, std::string const_type);
void printConstantTable(std::ofstream &out);

std::string searchTypedefTable(std::string Token);
void printType(std::ofstream &out);

void printTables(const std::string &inputFile);

SymbolTableEntry* lookup(std::string Token);
std::string getSizeOfType(const std::string& typeStr);
void debugEntry(SymbolTableEntry* entry);