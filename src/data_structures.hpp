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

struct Desc
{
	std::vector<int> inRegs;
	// enum {EAX = 0,EBX = 1,ECX = 2,EDX = 3,ESI = 4,EDI = 5,ESP = 6,EBP = 7 };
	int inStack = 0;
	bool inHeap = false;
};

typedef struct sym_entry
{
	std::string type;
	int size;
	bool init;
	int offset;
	std::map<std::string, sym_entry *> *entry;
	std::string access; //  field: "public", "private", "protected", etc.
	int isStatic = 0;
	bool isConst = false;
	bool isArray = false;
	bool isEnum = false;
	bool isGlobal = false;
	int nextUse = -1;
	int isLive = 0;
	Desc addrDesc;
	int isDerefer = 0; //For _ti_ = *x 
	std::vector<int> array_dims;
} sym_entry;

typedef std::map<std::string, sym_entry *> sym_table;
typedef std::map<std::string, std::pair<int, sym_table *>> struct_sym_table;
typedef std::map<std::string, std::pair<int, sym_table *>> class_sym_table;
typedef std::map<std::string, std::string> typ_table;

extern sym_table gst;
extern struct_sym_table struct_gst;
extern class_sym_table class_gst;
extern std::map<sym_table *, sym_table *> parent_table;
extern std::map<struct_sym_table *, struct_sym_table *> struct_parent_table;
extern std::map<class_sym_table *, class_sym_table *> class_parent_table;
extern bool inClassContext; // Flag to indicate if we're in a class definition
extern std::map<std::string, int> struct_size;
extern std::map<std::string, int> class_size;
extern std::map<std::string, std::vector<std::string>> func_arg;
extern int struct_offset;
extern int class_offset;
extern sym_table *curr_table; // store pointer of the current symbol table
extern sym_table *curr_structure;
extern sym_table *curr_class_structure;
extern struct_sym_table *curr_struct_table;
extern class_sym_table *curr_class_table;
extern std::stack<int> Goffset, Loffset, blockSz;
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
// std::string getSizeOfType(const std::string &typeStr);

void symTable_init();
sym_entry *createEntry(std::string type, int size, bool init, int offset, sym_table *ptr, std::string access = "", int isStatic = 0, bool isConst = false, bool isArray = false, bool isEnum = false,std::vector<int> array_dims = {});
void makeSymbolTable(std::string name, std::string f_type, bool offset_flag);
void removeFuncProto();
int updSymbolTable(std::string id, bool offset_flag);
sym_entry *lookup(std::string id);
sym_entry *currLookup(std::string id);
void insertKeywords();
void insert_std_func(std::string func_name, std::vector<std::string> type, std::string ret_type);
std::string getType(std::string id);
void createStructTable(bool union_f=false);
int insertStructAttr(std::string attr, std::string type, int size, bool init, bool isArray = false,std::vector<int> array_dims = {});
int printStructTable(std::string struct_name);
std::string StructAttrType(std::string struct_name, std::string id);
int StructAttrOffset(std::string struct_name, std::string id);
int findStruct(std::string struct_name);
int lookupStruct(std::string struct_name, std::string id);

void createClassTable();
int insertClassAttr(std::string attr, std::string type, int size, bool init, std::string access = "private",int isStatic = 0,bool isArray = false,std::vector<int> array_dims = {});
int printClassTable(std::string class_name);
std::string ClassAttrType(std::string class_name, std::string id);
int ClassAttrOffset(std::string class_name, std::string id);
std::string ClassAttrAccess(std::string className, std::string attr);
int inheritFromClass(std::string childClass, std::string parentClass);
int findClass(std::string class_name);
int lookupClass(std::string class_name, std::string &id);
sym_entry *lookupClassEntry(std::string class_name, std::string &id);
void createParamList();
void insertSymbol(sym_table &table, std::string id, std::string type, int size, bool is_init, sym_table *ptr, std::string access = "", int isStatic = 0, bool isConst = false, bool isArray = false, bool isEnum = false,std::vector<int> array_dims = {});
std::vector<std::string> getFuncArgs(std::string id);
std::string mangleFunctionName(const std::string &name, const std::vector<std::string> &paramTypes);
std::string getTypeCode(const std::string &type);
void updInit(std::string id);
void updTableSize(std::string id);
void insertFuncArg(std::string &func, std::vector<std::string> &arg);
void printSymbolTable(sym_table *table, std::string file_name);
int getSize(std::string id);
std::string lookupType(std::string a);
void insertType(std::string a, std::string b);

bool isMatching(std::string s1, std::string s2);

extern std::vector<std::string> lib_funcs;
extern std::set<std::string> called_lib_funcs;

extern std::map<std::string, std::string> global_init;

void paramInsert(sym_table &table, std::string id, std::string type, int size, bool is_init, sym_table *ptr,int isStatic = 0, bool isConst = false, bool isArray = false,std::vector<int> array_dims = {});
void clear_paramoffset();
sym_entry* lookupStruct_entry(std::string struct_name, std::string id);
sym_entry* lookupClass_entry(std::string class_name, std::string &id);
#endif // SYMBOL_TABLE_H
