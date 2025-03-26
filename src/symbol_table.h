#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ull unsigned long long
#define sym_file 0
#define sym_function 1
#define sym_block 2

typedef struct sym_entry
{
	string type;
	ull size;
	bool init;
	ull offset;
	map<string, sym_entry *> *entry;
	string access; //  field: "public", "private", "protected", etc.
} sym_entry;

typedef map<string, sym_entry *> sym_table;
typedef map<string, pair<ull, sym_table *>> struct_sym_table;
typedef map<string, pair<ull, sym_table *>> class_sym_table;
typedef map<string, string> typ_table;

extern sym_table gst;
extern struct_sym_table struct_gst;
extern class_sym_table class_gst;
extern map<sym_table *, sym_table *> parent_table;
extern map<struct_sym_table *, struct_sym_table *> struct_parent_table;
extern map<class_sym_table *, class_sym_table *> class_parent_table;

extern map<string, ull> struct_size;
extern map<string, ull> class_size;
extern map<string, vector<string>> func_arg;
extern ull struct_offset;
extern ull class_offset;
extern sym_table *curr_table; // store pointer of the current symbol table
extern sym_table *curr_structure;
extern sym_table *curr_class_structure;
extern struct_sym_table *curr_struct_table;
extern class_sym_table *curr_class_table;
extern stack<ull> Goffset, Loffset, blockSz;
extern int avl;

// map<string, string> typ_table;
extern typ_table typ_gst;
extern map<typ_table *, typ_table *> typ_parent_table;
extern typ_table *curr_typ;

void symTable_init();
sym_entry *createEntry(string type, ull size, bool init, ull offset, sym_table *ptr, string access = "");
void makeSymbolTable(string name, string f_type);
void removeFuncProto();
void updSymbolTable(string id);
sym_entry *lookup(string id);
sym_entry *currLookup(string id);
void insertKeywords();
string getType(string id);
void createStructTable();
int insertStructAttr(string attr, string type, ull size, bool init);
int printStructTable(string struct_name);
string StructAttrType(string struct_name, string id);
int findStruct(string struct_name);
int lookupStruct(string struct_name, string id);

void createClassTable();
int insertClassAttr(string attr, string type, ull size, bool init, string access = "private");
int printClassTable(string class_name);
string ClassAttrType(string class_name, string id);
int findClass(string class_name);
int lookupClass(string class_name, string id);

void createParamList();
void insertSymbol(sym_table &table, string id, string type, ull size, bool is_init, sym_table *ptr);
vector<string> getFuncArgs(string id);
void updInit(string id);
void updTableSize(string id);
void insertFuncArg(string &func, vector<string> &arg);
void printFuncArg();
void printSymbolTable(sym_table *table, string file_name);
ull getSize(string id);
string lookupType(string a);
void insertType(string a, string b);

#endif // SYMBOL_TABLE_H