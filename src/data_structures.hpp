#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstring>

typedef struct {
    std::string token;
    std::string data_type;
} SymbolTableEntry;

extern std::vector<SymbolTableEntry> SymbolTable;
extern std::vector<SymbolTableEntry> ConstantTable;
extern std::vector<std::pair<std::string,std::string>> typedefTable;

void addToSymbolTable(std::string Token, std::string Data_type);
void addStandardProceduresToSymbolTable();
void updateLastSymbolEntry();
void updateFuncSymbolEntry(int args);
void printSymbolTable();

void addToConstantTable(std::string constnt, std::string const_type);
void printConstantTable();

std::string searchTypedefTable(std::string Token);
void printType();