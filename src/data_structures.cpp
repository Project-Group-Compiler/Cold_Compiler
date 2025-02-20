#include<data_structures.hpp>
#include<fstream>
#include <iomanip>
#include <vector>

std::vector<SymbolTableEntry> SymbolTable;
std::vector<SymbolTableEntry> ConstantTable;
std::vector<std::pair<std::string,std::string>> typedefTable;

void addToSymbolTable(std::string Token, std::string Data_type) {
    SymbolTableEntry entry;
	entry.token=Token;
	entry.data_type=Data_type;
	SymbolTable.push_back(entry);
}

void addStandardProceduresToSymbolTable() {
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
        {"atoi", "Procedure"}
    };

	addToSymbolTable("     ", "     ");
    for (const auto& procedure : procedures) {
		addToSymbolTable(procedure.first, procedure.second);
    }
}

void updateLastSymbolEntry(){
	if (!SymbolTable.empty()) {
        SymbolTable.back().data_type+="[]";
    } else {
        std::cerr << "SymbolTable is already empty." << std::endl;
    }
}

void updateFuncSymbolEntry(int args){
	if (SymbolTable.size()>args) {
        SymbolTable[SymbolTable.size()-1-args].data_type="Procedure";
    } else {
        std::cerr << "SymbolTable is too small" << std::endl;
    }
}

void printSymbolTable() {
    printf("\nSymbol Table:\n");
    std::cout << std::left <<
    	std::setw(20) << "Token" 
     	<< std::setw(40) << "Data Type" << "\n"
     	<< std::string(80, '-') << "\n";
    for (int i = 0; i < SymbolTable.size(); i++) {
        std::cout << std::left <<
    	std::setw(20) << SymbolTable[i].token
     	<< std::setw(40) << SymbolTable[i].data_type << "\n";
    }
    printf("---------------------------\n");
}

void addToConstantTable(std::string constnt, std::string const_type){
	SymbolTableEntry entry;
	entry.token=constnt;
	entry.data_type=const_type;
	ConstantTable.push_back(entry);
}

void printConstantTable() {
    printf("\nConstant Table:\n");
    std::cout << std::left <<
    	std::setw(20) << "Constant" 
     	<< std::setw(40) << "Constant Type" << "\n"
     	<< std::string(80, '-') << "\n";
    for (int i = 0; i < ConstantTable.size(); i++) {
        std::cout << std::left <<
    	std::setw(20) << ConstantTable[i].token
     	<< std::setw(40) << ConstantTable[i].data_type << "\n";
    }
    printf("---------------------------\n");
}

std::string searchTypedefTable(std::string Token){
	for (int i=0; i<typedefTable.size(); i++){
		if(typedefTable[i].first==Token) return typedefTable[i].second;
	}
	return "";
}

void printType(){
	printf("\nTypedef Table:\n");
	std::cout << std::left <<
		std::setw(20) << "Definition" 
		<< std::setw(40) << "Keyword" << "\n"
		<< std::string(80, '-') << "\n";
	for (int i = 0; i < typedefTable.size(); i++) {
		std::cout << std::left <<
		std::setw(20) << typedefTable[i].first
		<< std::setw(40) << typedefTable[i].second << "\n";
	}
	printf("---------------------------\n");
}