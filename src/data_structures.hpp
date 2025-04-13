#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <fstream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <iomanip>
#include <iostream>

#define ll long long
#define ull unsigned long long
#define sym_file 0
#define sym_function 1
#define sym_block 2

struct TOKEN
{
	int line;
	int column;
	std::string token_type;
	std::string lexeme;
};

typedef struct sym_entry
{
	std::string type;
	ull size;
	bool init;
	ull offset;
	std::map<std::string, sym_entry *> *entry;
	std::string access; //  field: "public", "private", "protected", etc.
	bool isStatic = false;
	bool isConst = false;
	bool isArray = false;
	bool isEnum = false;
	bool isGlobal = false;
	int nextUse = -1; // TODO
	int isLive = 0;
} sym_entry;

typedef std::map<std::string, sym_entry *> sym_table;
typedef std::map<std::string, std::pair<ull, sym_table *>> struct_sym_table;
typedef std::map<std::string, std::pair<ull, sym_table *>> class_sym_table;
typedef std::map<std::string, std::string> typ_table;

extern sym_table gst;
extern struct_sym_table struct_gst;
extern class_sym_table class_gst;
extern std::map<sym_table *, sym_table *> parent_table;
extern std::map<struct_sym_table *, struct_sym_table *> struct_parent_table;
extern std::map<class_sym_table *, class_sym_table *> class_parent_table;
extern bool inClassContext; // Flag to indicate if we're in a class definition
extern std::map<std::string, ull> struct_size;
extern std::map<std::string, ull> class_size;
extern std::map<std::string, std::vector<std::string>> func_arg;
extern ull struct_offset;
extern ull class_offset;
extern sym_table *curr_table; // store pointer of the current symbol table
extern sym_table *curr_structure;
extern sym_table *curr_class_structure;
extern struct_sym_table *curr_struct_table;
extern class_sym_table *curr_class_table;
extern std::stack<ull> Goffset, Loffset, blockSz;
extern int avl;

extern typ_table typ_gst;
extern std::map<typ_table *, typ_table *> typ_parent_table;
extern typ_table *curr_typ;

// old functions
extern std::vector<std::pair<std::string, std::string>> typedefTable;

void print_error(const std::string &message);
std::string searchTypedefTable(std::string Token);
void printType(std::ofstream &out);
void printTables(const std::string &inputFile);
bool searchIdConst(std::string id);
std::string getSizeOfType(const std::string &typeStr);

void symTable_init();
sym_entry *createEntry(std::string type, ull size, bool init, ull offset, sym_table *ptr, std::string access = "", bool isStatic = false, bool isConst = false, bool isArray = false, bool isEnum = false);
void makeSymbolTable(std::string name, std::string f_type);
void removeFuncProto();
int updSymbolTable(std::string id);
sym_entry *lookup(std::string id);
sym_entry *currLookup(std::string id);
void insertKeywords();
void insert_std_func(std::string func_name, std::vector<std::string> type, std::string ret_type);
std::string getType(std::string id);
void createStructTable();
int insertStructAttr(std::string attr, std::string type, ull size, bool init);
int printStructTable(std::string struct_name);
std::string StructAttrType(std::string struct_name, std::string id);
int findStruct(std::string struct_name);
int lookupStruct(std::string struct_name, std::string id);

void createClassTable();
int insertClassAttr(std::string attr, std::string type, ull size, bool init, std::string access = "private");
int printClassTable(std::string class_name);
std::string ClassAttrType(std::string class_name, std::string id);
std::string ClassAttrAccess(std::string className, std::string attr);
int inheritFromClass(std::string childClass, std::string parentClass);
int findClass(std::string class_name);
int lookupClass(std::string class_name, std::string id);

void createParamList();
void insertSymbol(sym_table &table, std::string id, std::string type, ull size, bool is_init, sym_table *ptr, std::string access = "", bool isStatic = false, bool isConst = false, bool isArray = false, bool isEnum = false);
std::vector<std::string> getFuncArgs(std::string id);
std::string mangleFunctionName(const std::string &name, const std::vector<std::string> &paramTypes);
std::string getTypeCode(const std::string &type);
void updInit(std::string id);
void updTableSize(std::string id);
void insertFuncArg(std::string &func, std::vector<std::string> &arg);
void printSymbolTable(sym_table *table, std::string file_name);
ull getSize(std::string id);
std::string lookupType(std::string a);
void insertType(std::string a, std::string b);

bool isMatching(std::string s1, std::string s2);

extern std::vector<std::string> lib_funcs;
extern std::set<std::string> called_lib_funcs;

extern std::map<std::string, std::string> global_init;

#endif // SYMBOL_TABLE_H
