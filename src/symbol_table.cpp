#include "symbol_table.hpp"
#include <iostream>

// Global symbol table variables remain as before.
sym_table gst;
struct_sym_table struct_gst;
map<sym_table *, sym_table *> parent_table;
map<struct_sym_table *, struct_sym_table *> struct_parent_table;

map<string, ull> struct_size;
map<string, vector<string>> func_arg;
ull struct_offset;
sym_table *curr_table; // store pointer of the current symbol table
sym_table *curr_structure;
struct_sym_table *curr_struct_table;
stack<ull> Goffset, Loffset, blockSz;

bool inClassContext = false;  // Default: not in a class definition
typ_table typ_gst;
map<typ_table *, typ_table *> typ_parent_table;
typ_table *curr_typ;

class_sym_table class_gst;
map<class_sym_table *, class_sym_table *> class_parent_table;
map<string, ull> class_size;
ull class_offset;
sym_table *curr_class_structure;
class_sym_table *curr_class_table;
int class_count = 1;

int struct_count = 1;
int avl = 0;
int blockCnt = 1;

void symTable_init()
{
    // Initialize global stacks.
    Goffset.push(0);
    Loffset.push(0);
    blockSz.push(0);

    // Insert the global symbol table as the root.
    parent_table.insert(make_pair(&gst, nullptr));
    struct_parent_table.insert(make_pair(&struct_gst, nullptr));
    class_parent_table.insert(make_pair(&class_gst, nullptr));

    curr_table = &gst;
    curr_struct_table = &struct_gst;
    curr_class_table = &class_gst;
    curr_typ = &typ_gst;

    insertKeywords();
}

sym_entry *createEntry(string type, ull size, bool init, ull offset, sym_table *ptr, string access)
{
    sym_entry *new_sym = new (std::nothrow) sym_entry();
    if (!new_sym)
    {
        std::cerr << "Error: Memory allocation failed in createEntry.\n";
        exit(EXIT_FAILURE);
    }
    new_sym->type = type;
    new_sym->size = size;
    new_sym->init = init;
    new_sym->offset = offset;
    new_sym->entry = ptr;
    new_sym->access = access;
    return new_sym;
}

void makeSymbolTable(string name, string f_type)
{
    if (!avl)
    {
        sym_table *new_table = new (std::nothrow) sym_table;
        struct_sym_table *new_struct_table = new (std::nothrow) struct_sym_table;
        class_sym_table *new_class_table = new (std::nothrow) class_sym_table;
        typ_table *new_typ = new (std::nothrow) typ_table;
        if (!new_table || !new_struct_table || !new_typ || !new_class_table)
        {
            std::cerr << "Error: Memory allocation failed in makeSymbolTable.\n";
            exit(EXIT_FAILURE);
        }

        if (f_type != "")
        {
            insertSymbol(*curr_table, name, "FUNC_" + f_type, 0, true, new_table);
        }
        else
        {
            insertSymbol(*curr_table, name, "Block", 0, true, new_table);
            blockCnt++;
        }

        Goffset.push(0);
        blockSz.push(0);
        parent_table.insert(make_pair(new_table, curr_table));
        struct_parent_table.insert(make_pair(new_struct_table, curr_struct_table));//is this needed?
        class_parent_table.insert(make_pair(new_class_table, curr_class_table));//is this needed?
        typ_parent_table.insert(make_pair(new_typ, curr_typ));

        curr_table = new_table;
        curr_struct_table = new_struct_table;
        curr_class_table = new_class_table;
        curr_typ = new_typ;
    }
    else
    {
        avl = 0;
        if (parent_table.find(curr_table) == parent_table.end())
        {
            std::cerr << "Error: curr_table not found in parent_table during makeSymbolTable.\n";
            exit(EXIT_FAILURE);
        }
        // std::cout<<"Function name: " << name << std::endl;
        // std::cout<<"Function type: " << f_type << std::endl;
        (*parent_table[curr_table]).erase("dummyF_name");
        // Only add to parent table if not in class context
        if (!inClassContext) {
            (*parent_table[curr_table]).insert(make_pair(name, createEntry("FUNC_" + f_type, 0, true, Loffset.top(), curr_table)));
        }
        
        Loffset.pop();
    }
}

void removeFuncProto()
{
    avl = 0;
    updSymbolTable("dummyF_name");
    // Check for valid pointer before erasing
    if (curr_table && (*curr_table).find("dummyF_name") != (*curr_table).end())
    {
        sym_entry *entry = (*curr_table)["dummyF_name"];
        if (entry && entry->entry)
            parent_table.erase(entry->entry);
        (*curr_table).erase("dummyF_name");
    }
    else
    {
        std::cerr << "Warning: dummyF_name not found in removeFuncProto.\n";
    }
    if (!Loffset.empty())
        Loffset.pop();
}

void updSymbolTable(string id)
{
    if (Goffset.empty() || blockSz.empty())
    {
        std::cerr << "Error: Goffset or blockSz stack is empty in updSymbolTable.\n";
        return;
    }
    ull temp = Goffset.top();
    Goffset.pop();
    if (!Goffset.empty())
        Goffset.top() += temp;
    else
    {
        std::cerr << "Error: Goffset stack became empty in updSymbolTable.\n";
        exit(EXIT_FAILURE);
    }

    // Update current table pointers.
    if (parent_table.find(curr_table) == parent_table.end())
    {
        std::cerr << "Error: Parent table not found for current table in updSymbolTable.\n";
        exit(EXIT_FAILURE);
    }
    curr_table = parent_table[curr_table];
    curr_struct_table = struct_parent_table[curr_struct_table];
    curr_class_table = class_parent_table[curr_class_table];
    curr_typ = typ_parent_table[curr_typ];

    sym_entry *entry = lookup(id);
    if (entry)
        entry->size = blockSz.top();

    temp = blockSz.top();
    blockSz.pop();
    if (!blockSz.empty())
        blockSz.top() += temp;
    else
    {
        std::cerr << "Error: blockSz stack became empty in updSymbolTable.\n";
        exit(EXIT_FAILURE);
    }
}

sym_entry *lookup(string id)
{
    sym_table *temp = curr_table;
    while (temp)
    {
        if ((*temp).find(id) != (*temp).end())
            return (*temp)[id];

        // Added code: Check for overloaded function with base name
        if (id.find("FUNC_") != 0) { // If not already a mangled name
            string prefix = "FUNC_" + to_string(id.length()) + id + "_";
            for (auto& entry : (*temp)) {
                if (entry.first.find(prefix) == 0) {
                    // Found an overloaded function - return any version temporarily
                    return (*temp)[entry.first];
                }
            }
        }
        // Check if pointer exists in parent_table map.
        if (parent_table.find(temp) == parent_table.end())
            break;
        temp = parent_table[temp];
    }
    return nullptr;
}

sym_entry *currLookup(string id)
{
    if (!curr_table)
    {
        std::cerr << "Error: curr_table is NULL in currLookup.\n";
        return nullptr;
    }
    if ((*curr_table).find(id) == (*curr_table).end())
        return nullptr;
    return (*curr_table)[id];
}

void insertKeywords()
{
    vector<string> key_words = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "class", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
    vector<string> op = {"...", ">>=", "<<=", "+=", "-=", "*=", "/=", "%=", "&=", "^=", "|=", ">>", "<<", "++", "--", "->", "&&", "||", "<=", ">=", "==", "!=", ";", "{", "<%", "}", "%>", ",", ":", "=", "(", ")", "[", "<:", "]", ":>", ".", "&", "!", "~", "-", "+", "*", "/", "%", "<", ">", "^", "|", "?"};

    for (auto h : key_words)
    {
        insertSymbol(*curr_table, h, "keyword", 8, true, nullptr);
    }
    for (auto h : op)
    {
        insertSymbol(*curr_table, h, "operator", 8, true, nullptr);
    }
    
    // important io functions
	vector<string> type = {"char*", "..."};
	insert_imp_func("printf", type, "int");
	insert_imp_func("scanf", type, "int");

	// dynamic allocation functions
	type = {"int"};
	insert_imp_func("malloc", type, "void*");
	
    type = {"int", "int"};
	insert_imp_func("calloc", type, "void*");
	
    type = {"void*"};
	insert_imp_func("free", type, "void");

	// file io functions
	type = {"char*", "char*"};
	insert_imp_func("fopen", type, "FILE*");

	type = {"char*", "FILE*"};
	insert_imp_func("fputs", type, "int");
	
    type = {"char*", "int", "FILE*"};
	insert_imp_func("fgets", type, "int");
	
    type = {"FILE*"};
	insert_imp_func("fclose", type, "int");
	
    type = {"FILE*", "char*", "..."};
	insert_imp_func("fprintf", type, "int");
	
    type = {"FILE*", "char*", "..."};
	insert_imp_func("fscanf", type, "int");
	
    type = {"FILE*"};
	insert_imp_func("fgetc", type, "char");
	
    type = {"char", "FILE*"};
	insert_imp_func("fputc", type, "char");

	// string Functions
	type = {"char*"};
	insert_imp_func("strlen", type, "int");
	
    type = {"char*", "char*"};
	insert_imp_func("strcmp", type, "int");
	
    type = {"char*", "char*", "int"};
	insert_imp_func("strncmp", type, "int");
	
    type = {"char*", "char*"};
	insert_imp_func("strcpy", type, "char*");
	insert_imp_func("strcat", type, "char*");

    // variable arguments functions
    type = {"va_list", "int"};
    insert_imp_func("va_start", type, "void");

    type = {"va_list", "char*"};
    insert_imp_func("va_arg", type, "int");
    
    type = {"va_list"};
    insert_imp_func("va_end", type, "void");
}

void insert_imp_func(string func_name, vector<string> type, string ret_type){
	insertSymbol(*curr_table, func_name, "FUNC_"+ret_type, 4, 0, nullptr);
	//func_arg.insert({func_name, make_pair("FUNC_"+ret_type, type)});
    func_arg.insert({func_name, type});
	//func_usage_map.insert({func_name, 0});
}


string getType(string id){
    sym_entry* entry = lookup(id);
    string ret = "";
    if (entry)
        ret += entry->type;
    return ret;
}

void createStructTable()
{
    sym_table *new_table = new (std::nothrow) sym_table;
    if (!new_table)
    {
        std::cerr << "Error: Memory allocation failed in createStructTable.\n";
        exit(EXIT_FAILURE);
    }
    curr_structure = new_table;
    struct_offset = 0;
}

// insert struct attributes in struct symbol table
int insertStructAttr(string attr, string type, ull size, bool init)
{
    if (!curr_structure)
    {
        std::cerr << "Error: curr_structure is NULL in insertStructAttr.\n";
        return 0;
    }
    if ((*curr_structure).find(attr) == (*curr_structure).end())
    {
        if (!blockSz.empty())
            blockSz.top() += size;
        if (!Goffset.empty())
            Goffset.top() += size;
        (*curr_structure).insert(make_pair(attr, createEntry(type, size, init, struct_offset, nullptr)));
        struct_offset += size;
        return 1;
    }
    return 0;
}

int printStructTable(string struct_name)
{
    if (!curr_struct_table)
    {
        std::cerr << "Error: curr_struct_table is NULL in printStructTable.\n";
        return 0;
    }
    if ((*curr_struct_table).find(struct_name) == (*curr_struct_table).end())
    {
        (*curr_struct_table).insert(make_pair(struct_name, make_pair(struct_offset, curr_structure)));
        printSymbolTable(curr_structure, struct_name + "_" + to_string(struct_count) + ".csv"); // prints structure symbol table
        struct_count++;
        return 1;
    }
    return 0;
}

string StructAttrType(string struct_name, string id)
{
    struct_sym_table *temp = curr_struct_table;
    while (temp && ((*temp).find(struct_name) == (*temp).end()))
    {
        if (struct_parent_table.find(temp) == struct_parent_table.end())
        {
            std::cerr << "Error: struct_name " << struct_name << " not found in StructAttrType.\n";
            return "";
        }
        temp = struct_parent_table[temp];
    }
    if (!temp)
    {
        std::cerr << "Error: No valid struct_sym_table found in StructAttrType.\n";
        return "";
    }
    sym_table *table = (*temp)[struct_name].second;
    if (!table || (*table).find(id) == (*table).end())
    {
        std::cerr << "Error: Identifier " << id << " not found in struct " << struct_name << ".\n";
        return "";
    }
    return ((*table)[id]->type);
}

int findStruct(string struct_name)
{
    struct_sym_table *temp = curr_struct_table;
    while (temp)
    {
        if ((*temp).find(struct_name) != (*temp).end())
            return 1;
        if (struct_parent_table.find(temp) == struct_parent_table.end())
            break;
        temp = struct_parent_table[temp];
    }
    return 0;
}

int lookupStruct(string struct_name, string id)
{
    struct_sym_table *temp = curr_struct_table;
    while (temp)
    {
        if ((*temp).find(struct_name) != (*temp).end())
        {
            sym_table *table = (*temp)[struct_name].second;
            if (table && (*table).find(id) != (*table).end())
                return 1; // found
            else
                return 0; // struct doesn't contain id
        }
        if (struct_parent_table.find(temp) == struct_parent_table.end())
            break;
        temp = struct_parent_table[temp];
    }
    return -1; // struct table not found
}

ull getStructsize(string struct_name)
{
    struct_sym_table *temp = curr_struct_table;
    while (temp)
    {
        if ((*temp).find(struct_name) != (*temp).end())
        {
            return (*temp)[struct_name].first;
        }
        if (struct_parent_table.find(temp) == struct_parent_table.end())
            break;
        temp = struct_parent_table[temp];
    }
    return 0;
}

void createClassTable()
{
    sym_table *new_table = new (std::nothrow) sym_table;
    if (!new_table)
    {
        std::cerr << "Error: Memory allocation failed in createClassTable.\n";
        exit(EXIT_FAILURE);
    }
    curr_class_structure = new_table;
    class_offset = 0;
}
int insertClassAttr(string attr, string type, ull size, bool init, string access)
{
    if (!curr_class_structure)
    {
        std::cerr << "Error: curr_class_structure is NULL in insertClassAttr.\n";
        return 0;
    }
    if ((*curr_class_structure).find(attr) == (*curr_class_structure).end())
    {
        if (!blockSz.empty())
            blockSz.top() += size;
        if (!Goffset.empty())
            Goffset.top() += size;
        (*curr_class_structure).insert(make_pair(attr, createEntry(type, size, init, class_offset, nullptr, access)));
        class_offset += size;
        return 1;
    }
    return 0;
}
int printClassTable(string class_name)
{
    if (!curr_class_table)
    {
        std::cerr << "Error: curr_class_table is NULL in printClassTable.\n";
        return 0;
    }
    if ((*curr_class_table).find(class_name) == (*curr_class_table).end())
    {
        (*curr_class_table).insert(make_pair(class_name, make_pair(class_offset, curr_class_structure)));
        printSymbolTable(curr_class_structure, class_name + "_" + to_string(class_count) + ".csv"); // prints class symbol table
        class_count++;
        return 1;
    }
    return 0;
}
string ClassAttrType(string class_name, string id)
{
    class_sym_table *temp = curr_class_table;
    while (temp && ((*temp).find(class_name) == (*temp).end()))
    {
        if (class_parent_table.find(temp) == class_parent_table.end())
        {
            std::cerr << "Error: class_name " << class_name << " not found in ClassAttrType.\n";
            return "";
        }
        temp = class_parent_table[temp];
    }
    if (!temp)
    {
        std::cerr << "Error: No valid class_sym_table found in ClassAttrType.\n";
        return "";
    }
    sym_table *table = (*temp)[class_name].second;
    if (!table || (*table).find(id) == (*table).end())
    {
        std::cerr << "Error: Identifier " << id << " not found in class " << class_name << ".\n";
        return "";
    }
    return ((*table)[id]->type);
}
string ClassAttrAccess(string className, string attr) {
    // Search through class table hierarchy
    class_sym_table* temp = curr_class_table;
    
    while (temp) {
        // Check if this class table has the class we're looking for
        if ((*temp).find(className) != (*temp).end()) {
            sym_table* classTable = (*temp)[className].second;
            if (classTable && (*classTable).find(attr) != (*classTable).end()) {
                return (*classTable)[attr]->access;
            }
            return ""; // Member not found in this class
        }
        
        // Move up to parent class table if exists
        if (class_parent_table.find(temp) == class_parent_table.end()) {
            break;
        }
        temp = class_parent_table[temp];
    }
    
    return ""; // Class or member not found
}
int findClass(string class_name)
{
    class_sym_table *temp = curr_class_table;
    while (temp)
    {
        if ((*temp).find(class_name) != (*temp).end())
            return 1;
        if (class_parent_table.find(temp) == class_parent_table.end())
            break;
        temp = class_parent_table[temp];
    }
    return 0;
}
int lookupClass(string class_name, string id)
{
    class_sym_table *temp = curr_class_table;
    while (temp)
    {
        if ((*temp).find(class_name) != (*temp).end())
        {
            sym_table *table = (*temp)[class_name].second;
            if (table && (*table).find(id) != (*table).end())
                return 1; // found
            else
                return 0; // class doesn't contain id
        }
        if (class_parent_table.find(temp) == class_parent_table.end())
            break;
        temp = class_parent_table[temp];
    }
    return -1; // class table not found
}
ull getClassSize(string class_name)
{
    class_sym_table *temp = curr_class_table;
    while (temp)
    {
        if ((*temp).find(class_name) != (*temp).end())
        {
            return (*temp)[class_name].first;
        }
        if (class_parent_table.find(temp) == class_parent_table.end())
            break;
        temp = class_parent_table[temp];
    }
    return 0;
}
int inheritFromClass(string childClassName, string parentClassName) {
    // Find the parent class symbol table
    class_sym_table* temp = curr_class_table;
    sym_table* parentClassTable = nullptr;
    
    // Search for parent class in the class hierarchy
    while (temp) {
        if ((*temp).find(parentClassName) != (*temp).end()) {
            parentClassTable = (*temp)[parentClassName].second;
            break;
        }
        
        if (class_parent_table.find(temp) == class_parent_table.end()) {
            break;
        }
        temp = class_parent_table[temp];
    }
    
    // If parent class not found, report error
    if (!parentClassTable) {
        return 0; // Parent class not found
    }
    
    // Copy public members from parent to child class
    for (auto it : (*parentClassTable)) {
        string memberName = it.first;
        sym_entry* member = it.second;
        
        // Only inherit public and protected members
        if (member->access == "public" || member->access == "protected") {
            // Check if it's a method (starts with FUNC_)
            if (memberName.find("FUNC_") == 0) {
                // This is a method - needs special handling
                
                // Parse the original method name to extract components
                size_t firstUnderPos = memberName.find('_', 5);  // Skip "FUNC_"
                if (firstUnderPos != string::npos) {
                    // Extract parent class name length and name
                    string parentLenStr = memberName.substr(5, firstUnderPos - 5);
                    std::cout<<"Parent length: " << parentLenStr << std::endl;
                    int parentNameLen = stoi(parentLenStr);
                    
                    // Extract method name and signature (everything after parent class name)
                    string methodSuffix = memberName.substr(5 + parentNameLen + 1 + 1);
                    std::cout<<"Method suffix: " << methodSuffix << std::endl;
                    // Create new mangled name with child class
                    string childClassNameBase = childClassName.substr(6); // Remove "CLASS_" prefix
                    std::cout<<"Child class name base: " << childClassNameBase << std::endl;
                    string newMangledName = "FUNC_" + to_string(childClassNameBase.length()) + 
                                           childClassNameBase + "_" + methodSuffix;
                    std::cout<<"New mangled name: " << newMangledName << std::endl;
                    
                    // Create a copy of the method's symbol entry
                    insertClassAttr(newMangledName, member->type, member->size, 
                                  member->init, member->access);
                    
                    // If the method has a symbol table (for function body), copy it
                    if (member->entry) {
                        // Create a new symbol table for the inherited method
                        sym_table* newMethodTable = new sym_table(*(member->entry));
                        
                        // Update the entry pointer in the new method
                        (*curr_class_structure)[newMangledName]->entry = newMethodTable;
                        
                        // Register this in the parent_table map
                        parent_table[newMethodTable] = curr_table;
                        
                        // Copy function arguments if this method has parameters
                        if (func_arg.find(memberName) != func_arg.end()) {
                            func_arg[newMangledName] = func_arg[memberName];
                        }
                    }
                }
            } else {
                // Regular member (non-method) - copy as is
                if ((*curr_class_structure).find(memberName) == (*curr_class_structure).end()) {
                    insertClassAttr(memberName, member->type, member->size, 
                                 member->init, member->access);
                }
            }
        }
    }
    
    return 1; // Success
}
void createParamList()
{
    if (!Goffset.empty())
        Loffset.push(Goffset.top());
    else
    {
        std::cerr << "Error: Goffset stack empty in createParamList.\n";
        exit(EXIT_FAILURE);
    }
    makeSymbolTable("dummyF_name", "");
    avl = 1;
}

void insertSymbol(sym_table &table, string id, string type, ull size, bool is_init, sym_table *ptr)
{
    table.insert(make_pair(id, createEntry(type, size, is_init, Goffset.top(), ptr)));
    if (!blockSz.empty())
        blockSz.top() += size;
    else
    {
        std::cerr << "Error: blockSz stack empty in insertSymbol.\n";
        exit(EXIT_FAILURE);
    }
    if (!Goffset.empty())
        Goffset.top() += size;
    else
    {
        std::cerr << "Error: Goffset stack empty in insertSymbol.\n";
        exit(EXIT_FAILURE);
    }
}

vector<string> getFuncArgs(string id)
{
    vector<string> temp;
    temp.push_back("#NO_FUNC");
    if (func_arg.find(id) != func_arg.end())
        return func_arg[id];
    else
        return temp;
}
// Add to symbol_table.cpp
string getTypeCode(const string& type) {
    // Convert full type names to single-character codes
    if (type == "int") return "i";
    if (type == "float") return "f";
    if (type == "double") return "d";
    if (type == "char") return "c";
    if (type == "void") return "v";
    if (type == "bool") return "b";
    if (type.find("*") != string::npos) return "p"; // All pointers
    // Add more types as needed
    return "u"; // Unknown type
}

string mangleFunctionName(const string& name, const vector<string>& paramTypes) {
    string result = "FUNC_" + to_string(name.length()) + name + "_";
    
    // If no parameters, use 'v' for void
    if (paramTypes.empty()) {
        result += "v";
        return result;
    }
    
    // Add type codes for each parameter
    for (const auto& type : paramTypes) {
        result += getTypeCode(type);
    }
    
    return result;
}

void updInit(string id)
{
    sym_entry *entry = lookup(id);
    if (entry)
        entry->init = true;
    else
        std::cerr << "Warning: updInit called with unknown id " << id << ".\n";
}

void updTableSize(string id)
{
    sym_entry *entry = lookup(id);
    if (entry)
        entry->size = blockSz.top();
    else
        std::cerr << "Warning: updTableSize called with unknown id " << id << ".\n";
}

void insertFuncArg(string &func, vector<string> &arg)
{
    func_arg.insert(make_pair(func, arg));
}

void insertType(string a, string b)
{
    if (curr_typ)
    {
        if ((*curr_typ).find(b) == (*curr_typ).end())
        {
            (*curr_typ).insert(make_pair(a, b));
        }
        else
        {
            (*curr_typ).insert(make_pair(a, (*curr_typ)[b]));
        }
    }
    else
    {
        std::cerr << "Error: curr_typ is NULL in insertType.\n";
    }
}

string lookupType(string a)
{
    typ_table *temp = curr_typ;
    while (temp)
    {
        if ((*temp).find(a) != (*temp).end())
            return (*temp)[a];
        if (typ_parent_table.find(temp) == typ_parent_table.end())
            break;
        temp = typ_parent_table[temp];
    }
    return "";
}

void printSymbolTable(sym_table *table, string file_name)
{
    if (!table)
    {
        std::cerr << "Error: NULL table passed to printSymbolTable.\n";
        return;
    }
    FILE *file = fopen(file_name.c_str(), "w");
    if (!file)
    {
        std::cerr << "Error: Cannot open " << file_name << " for writing.\n";
        return;
    }

    // Check if this is a class symbol table (filename starts with "CLASS_")
    bool isClassTable = (file_name.find("CLASS_") == 0);

    // Add the Access column only for class tables
    if (isClassTable)
    {
        fprintf(file, "Name, Type, Size, isInitialized, Offset, Access\n");
        for (auto it : (*table))
        {
            fprintf(file, "%s, %s, %lld, %d, %lld, %s\n",
                    it.first.c_str(),
                    it.second->type.c_str(),
                    it.second->size,
                    it.second->init,
                    it.second->offset,
                    it.second->access.c_str()); // Print access modifier for class members
        }
    }
    else
    {
        // Original format for non-class tables
        fprintf(file, "Name, Type, Size, isInitialized, Offset\n");
        for (auto it : (*table))
        {
            fprintf(file, "%s, %s, %lld, %d, %lld\n",
                    it.first.c_str(),
                    it.second->type.c_str(),
                    it.second->size,
                    it.second->init,
                    it.second->offset);
        }
    }

    fclose(file);
}

ull getSize(string id)
{
    if (struct_size.find(id) != struct_size.end())
        return getStructsize(id);
    if (class_size.find(id) != class_size.end())
        return getClassSize(id);
    if (id == "char")
        return sizeof(char);
    if (id == "short")
        return sizeof(short);
    if (id == "short int")
        return sizeof(short int);
    if (id == "int")
        return sizeof(int);
    if (id == "long int")
        return sizeof(long int);
    if (id == "long long")
        return sizeof(long long);
    if (id == "long long int")
        return sizeof(long long int);
    if (id == "float")
        return sizeof(float);
    if (id == "double")
        return sizeof(double);
    if (id == "long double")
        return sizeof(long double);
    if (id == "signed short int")
        return sizeof(signed short int);
    if (id == "signed int")
        return sizeof(signed int);
    if (id == "signed long int")
        return sizeof(signed long int);
    if (id == "signed long long")
        return sizeof(signed long long);
    if (id == "signed long long int")
        return sizeof(signed long long int);
    if (id == "unsigned short int")
        return sizeof(unsigned short int);
    if (id == "unsigned int")
        return sizeof(unsigned int);
    if (id == "unsigned long int")
        return sizeof(unsigned long int);
    if (id == "unsigned long long")
        return sizeof(unsigned long long);
    if (id == "unsigned long long int")
        return sizeof(unsigned long long int);
    if(id == "va_list") return sizeof(va_list);
    return 8; // for any pointer type
}
