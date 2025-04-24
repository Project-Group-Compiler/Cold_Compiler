#include "data_structures.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <algorithm>

extern bool print_symtab;
extern std::string outputDir;

// Global symbol table variables remain as before.
sym_table gst;
struct_sym_table struct_gst;
std::map<sym_table *, sym_table *> parent_table;
std::map<struct_sym_table *, struct_sym_table *> struct_parent_table;

std::map<std::string, std::vector<std::string>> func_arg;
int struct_offset;
sym_table *curr_table; // store pointer of the current symbol table
sym_table *curr_structure;
struct_sym_table *curr_struct_table;
std::stack<int> Goffset, Loffset, blockSz;

bool inClassContext = false; // Default: not in a class definition
typ_table typ_gst;
std::map<typ_table *, typ_table *> typ_parent_table;
typ_table *curr_typ;

class_sym_table class_gst;
std::map<class_sym_table *, class_sym_table *> class_parent_table;
int class_offset;
sym_table *curr_class_structure;
class_sym_table *curr_class_table;
int class_count = 1;

int struct_count = 1;
int avl = 0;
int blockCnt = 1;

#define LIB_FUNC_LIST "scanf", "printf", "malloc", "calloc", "free",           \
                      "fopen", "fputs", "fgets", "fclose", "fprintf",          \
                      "fscanf", "fgetc", "fputc", "strlen", "strcmp",          \
                      "strncmp", "strcpy", "strcat", "va_start", "va_arg",     \
                      "va_end", "fread", "fwrite", "fseek", "ftell", "rewind", \
                      "memcpy", "memset", "atoi", "atof", "abs", "sin", "cos", \
                      "tan", "exp", "log", "log10", "pow", "sqrt", "ceil",     \
                      "floor", "fmod", "round", "fabs", "fmax", "fmin"

const char *func_array[] = {LIB_FUNC_LIST};
std::vector<std::string> lib_funcs(std::begin(func_array), std::end(func_array));
std::set<std::string> called_lib_funcs;

std::map<std::string, std::string> global_init;

std::vector<std::pair<std::string, std::string>> typedefTable;
extern std::string outputDir;

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
    {
        std::cerr << "Typedef table is empty\n";
        return;
    }

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

void printTables(const std::string &inputFile)
{
    std::ofstream out(outputDir + inputFile + "_tables.txt");
    if (!out)
    {
        print_error("cannot open " + outputDir + inputFile + "_tables.txt");
        return;
    }
    printType(out);
}
bool searchIdConst(std::string id)
{
    sym_entry *n = lookup(id);
    if (n)
    {
        bool s = n->isConst;
        return s;
    }
    return 0;
}

void symTable_init()
{
    // Initialize global stacks.
    Goffset.push(0);
    Loffset.push(0);
    blockSz.push(0);

    // Insert the global symbol table as the root.
    parent_table.insert(std::make_pair(&gst, nullptr));
    struct_parent_table.insert(std::make_pair(&struct_gst, nullptr));
    class_parent_table.insert(std::make_pair(&class_gst, nullptr));

    curr_table = &gst;
    curr_struct_table = &struct_gst;
    curr_class_table = &class_gst;
    curr_typ = &typ_gst;

    insertKeywords();
}

sym_entry *createEntry(std::string type, int size, bool init, int offset, sym_table *ptr, std::string access, int isStatic, bool isConst, bool isArray, bool isEnum, std::vector<int> array_dims)
{
    sym_entry *new_sym = new (std::nothrow) sym_entry;

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
    new_sym->isStatic = isStatic;
    new_sym->isConst = isConst;
    new_sym->isArray = isArray;
    new_sym->isEnum = isEnum;
    new_sym->array_dims = array_dims; // Initialize array dimensions
    return new_sym;
}

void makeSymbolTable(std::string name, std::string f_type, bool offset_flag)
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
        if (offset_flag)
            blockSz.push(0);
        parent_table.insert(std::make_pair(new_table, curr_table));
        struct_parent_table.insert(std::make_pair(new_struct_table, curr_struct_table)); // is this needed?
        class_parent_table.insert(std::make_pair(new_class_table, curr_class_table));    // is this needed?
        typ_parent_table.insert(std::make_pair(new_typ, curr_typ));

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
        (*parent_table[curr_table]).erase("dummyF_name");
        // Only add to parent table if not in class context
        if (!inClassContext)
        {
            (*parent_table[curr_table]).insert(std::make_pair(name, createEntry("FUNC_" + f_type, 0, true, Loffset.top(), curr_table)));
        }

        Loffset.pop();
    }
}

void removeFuncProto()
{
    avl = 0;
    updSymbolTable("dummyF_name", true);
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

int updSymbolTable(std::string id, bool offset_flag)
{
    if (Goffset.empty() || blockSz.empty())
    {
        std::cerr << "Error: Goffset or blockSz stack is empty in updSymbolTable.\n";
        return 0;
    }
    int temp = Goffset.top();
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

    if (parent_table.find(curr_table) != parent_table.end())
        curr_table = parent_table[curr_table];
    if (struct_parent_table.find(curr_struct_table) != struct_parent_table.end())
        curr_struct_table = struct_parent_table[curr_struct_table];
    if (class_parent_table.find(curr_class_table) != class_parent_table.end())
        curr_class_table = class_parent_table[curr_class_table];
    if (typ_parent_table.find(curr_typ) != typ_parent_table.end())
        curr_typ = typ_parent_table[curr_typ];

    if (sym_entry *entry = lookup(id))
    {
        entry->size = blockSz.top();
    }
    else if (id.find("FUNC_") == 0)
    {
        return blockSz.top(); // sending the size of class method
    }
    if (offset_flag)
    {
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
    return 0;
}

sym_entry *lookup(std::string id)
{
    sym_table *temp = curr_table;
    while (temp)
    {
        if ((*temp).find(id) != (*temp).end())
            return (*temp)[id];

        // Added code: Check for overloaded function with base name
        if (id.find("FUNC_") != 0)
        { // If not already a mangled name
            std::string prefix = "FUNC_" + std::to_string(id.length()) + id + "_";
            for (auto &entry : (*temp))
            {
                if (entry.first.find(prefix) == 0)
                {
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

sym_entry *currLookup(std::string id)
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
    // important io functions
    insert_std_func("printf", {"char*", "..."}, "int");
    insert_std_func("scanf", {"char*", "..."}, "int");

    // dynamic allocation functions
    insert_std_func("malloc", {"int"}, "void*");
    insert_std_func("calloc", {"int", "int"}, "void*");
    insert_std_func("free", {"void*"}, "void");

    // file io functions
    insert_std_func("fopen", {"char*", "char*"}, "file*");
    insert_std_func("fputs", {"char*", "file*"}, "int");
    insert_std_func("fgets", {"char*", "int", "file*"}, "int");
    insert_std_func("fclose", {"file*"}, "int");
    insert_std_func("fprintf", {"file*", "char*", "..."}, "int");
    insert_std_func("fscanf", {"file*", "char*", "..."}, "int");
    insert_std_func("fgetc", {"file*"}, "char");
    insert_std_func("fputc", {"char", "file*"}, "char");
    insert_std_func("fread", {"void*", "int", "int", "file*"}, "int");
    insert_std_func("fwrite", {"void*", "int", "int", "file*"}, "int");
    insert_std_func("fseek", {"file*", "int", "int"}, "int");
    insert_std_func("ftell", {"file*"}, "int");
    insert_std_func("rewind", {"file*"}, "void");

    // string Functions
    insert_std_func("strlen", {"char*"}, "int");
    insert_std_func("strcmp", {"char*", "char*"}, "int");
    insert_std_func("strncmp", {"char*", "char*", "int"}, "int");
    insert_std_func("strcpy", {"char*", "char*"}, "char*");
    insert_std_func("strcat", {"char*", "char*"}, "char*");
    insert_std_func("memcpy", {"void*", "void*", "int"}, "void*");
    insert_std_func("memset", {"void*", "int", "int"}, "void*");

    // conversion functions
    insert_std_func("atoi", {"char*"}, "int");
    insert_std_func("atof", {"char*"}, "float");

    // math functions
    insert_std_func("abs", {"int"}, "int");
    insert_std_func("fabs", {"float"}, "float");
    insert_std_func("sin", {"float"}, "float");
    insert_std_func("cos", {"float"}, "float");
    insert_std_func("tan", {"float"}, "float");
    insert_std_func("exp", {"float"}, "float");
    insert_std_func("log", {"float"}, "float");
    insert_std_func("log10", {"float"}, "float");
    insert_std_func("pow", {"float", "float"}, "float");
    insert_std_func("sqrt", {"float"}, "float");
    insert_std_func("ceil", {"float"}, "float");
    insert_std_func("floor", {"float"}, "float");
    insert_std_func("round", {"float"}, "float");
    insert_std_func("fmod", {"float", "float"}, "float");
    insert_std_func("fmax", {"float", "float"}, "float");
    insert_std_func("fmin", {"float", "float"}, "float");

    // variable arguments functions
    insert_std_func("va_start", {"va_list", "int"}, "void");
    insert_std_func("va_arg", {"va_list", "char*"}, "int");
    insert_std_func("va_end", {"va_list"}, "void");
}

void insert_std_func(std::string func_name, std::vector<std::string> type, std::string ret_type)
{
    insertSymbol(*curr_table, func_name, "FUNC_" + ret_type, 4, 0, nullptr);
    func_arg.insert({func_name, type});
}

std::string getType(std::string id)
{
    sym_entry *entry = lookup(id);
    std::string ret = "";
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
int insertStructAttr(std::string attr, std::string type, int size, bool init, bool isArray, std::vector<int> array_dims)
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
        if (type != "char")
        {
            if (struct_offset % 4 != 0)
            {
                int padding = (4 - (struct_offset % 4));
                struct_offset += padding;
                if (!blockSz.empty())
                    blockSz.top() += padding;
                if (!Goffset.empty())
                    Goffset.top() += padding;
            }
        }
        (*curr_structure).insert(std::make_pair(attr, createEntry(type, size, init, struct_offset, nullptr, "", 0, 0, isArray, 0, array_dims)));
        struct_offset += size;
        return 1;
    }
    return 0;
}

int printStructTable(std::string struct_name)
{
    if (!curr_struct_table)
    {
        std::cerr << "Error: curr_struct_table is NULL in printStructTable.\n";
        return 0;
    }
    if ((*curr_struct_table).find(struct_name) == (*curr_struct_table).end())
    {
        if (struct_offset % 4 != 0)
            struct_offset += 4 - (struct_offset % 4); // padding
        (*curr_struct_table).insert(std::make_pair(struct_name, std::make_pair(struct_offset, curr_structure)));
        printSymbolTable(curr_structure, struct_name + "_" + std::to_string(struct_count) + ".csv"); // prints structure symbol table
        struct_count++;
        return 1;
    }
    return 0;
}

std::string StructAttrType(std::string struct_name, std::string id)
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

int StructAttrOffset(std::string struct_name, std::string id)
{
    struct_sym_table *temp = curr_struct_table;
    while (temp && ((*temp).find(struct_name) == (*temp).end()))
    {
        if (struct_parent_table.find(temp) == struct_parent_table.end())
        {
            std::cerr << "Error: struct_name " << struct_name << " not found in StructAttrOffset.\n";
            return -1;
        }
        temp = struct_parent_table[temp];
    }
    if (!temp)
    {
        std::cerr << "Error: No valid struct_sym_table found in StructAttrOffset.\n";
        return -1;
    }
    sym_table *table = (*temp)[struct_name].second;
    if (!table || (*table).find(id) == (*table).end())
    {
        std::cerr << "Error: Identifier " << id << " not found in struct " << struct_name << ".\n";
        return -1;
    }
    return ((*table)[id]->offset);
}

int findStruct(std::string struct_name)
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

int lookupStruct(std::string struct_name, std::string id)
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

int getStructsize(std::string struct_name)
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
int insertClassAttr(std::string attr, std::string type, int size, bool init, std::string access, int isStatic, bool isArray, std::vector<int> array_dims)
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
        if (type != "char")
        {
            if (class_offset % 4 != 0)
            {
                int padding = (4 - (class_offset % 4));
                class_offset += padding;
                if (!blockSz.empty())
                    blockSz.top() += padding;
                if (!Goffset.empty())
                    Goffset.top() += padding;
            }
        }
        (*curr_class_structure).insert(std::make_pair(attr, createEntry(type, size, init, class_offset, nullptr, access, isStatic, 0, isArray, 0, array_dims)));
        class_offset += size;
        return 1;
    }
    return 0;
}
int printClassTable(std::string class_name)
{
    if (!curr_class_table)
    {
        std::cerr << "Error: curr_class_table is NULL in printClassTable.\n";
        return 0;
    }
    if ((*curr_class_table).find(class_name) == (*curr_class_table).end())
    {
        if (class_offset % 4 != 0)
            class_offset += 4 - (class_offset % 4); // padding
        (*curr_class_table).insert(std::make_pair(class_name, std::make_pair(class_offset, curr_class_structure)));
        printSymbolTable(curr_class_structure, class_name + "_" + std::to_string(class_count) + ".csv"); // prints class symbol table
        class_count++;
        return 1;
    }
    return 0;
}
std::string ClassAttrType(std::string class_name, std::string id)
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

int ClassAttrOffset(std::string class_name, std::string id)
{
    class_sym_table *temp = curr_class_table;
    while (temp && ((*temp).find(class_name) == (*temp).end()))
    {
        if (class_parent_table.find(temp) == class_parent_table.end())
        {
            std::cerr << "Error: class_name " << class_name << " not found in ClassAttrOffset.\n";
            return -1;
        }
        temp = class_parent_table[temp];
    }
    if (!temp)
    {
        std::cerr << "Error: No valid class_sym_table found in ClassAttrOffset.\n";
        return -1;
    }
    sym_table *table = (*temp)[class_name].second;
    if (!table || (*table).find(id) == (*table).end())
    {
        std::cerr << "Error: Identifier " << id << " not found in class " << class_name << ".\n";
        return -1;
    }
    return ((*table)[id]->offset);
}

std::string ClassAttrAccess(std::string className, std::string attr)
{
    // Search through class table hierarchy
    class_sym_table *temp = curr_class_table;

    while (temp)
    {
        // Check if this class table has the class we're looking for
        if ((*temp).find(className) != (*temp).end())
        {
            sym_table *classTable = (*temp)[className].second;
            if (classTable && (*classTable).find(attr) != (*classTable).end())
            {
                return (*classTable)[attr]->access;
            }
            return ""; // Member not found in this class
        }

        // Move up to parent class table if exists
        if (class_parent_table.find(temp) == class_parent_table.end())
        {
            break;
        }
        temp = class_parent_table[temp];
    }

    return ""; // Class or member not found
}
int findClass(std::string class_name)
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
int lookupClass(std::string class_name, std::string &id)
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
            {
                // check if id is in parent class
                if (id.find("FUNC_") == 0)
                {
                    // for example FUNC_6Person_6getAge_v change this to 6getAge_v
                    id = id.substr(5);
                    id = id.substr(id.find("_") + 1);
                    // check if id is in this class as suffix of entry
                    for (auto it : (*table))
                    {
                        std::string key = it.first;
                        if (key.length() >= id.length() &&
                            key.substr(key.length() - id.length()) == id)
                        {
                            id = it.first; // to pass parent class name function to parser.y
                            return 1;
                        }
                    }
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
        if (class_parent_table.find(temp) == class_parent_table.end())
            break;
        temp = class_parent_table[temp];
    }
    return -1; // class table not found
}

sym_entry *lookupClassEntry(std::string class_name, std::string &id)
{
    class_sym_table *temp = curr_class_table;
    while (temp)
    {
        if ((*temp).find(class_name) != (*temp).end())
        {
            sym_table *table = (*temp)[class_name].second;
            if (table && (*table).find(id) != (*table).end())
                return (*table)[id]; // found
            else
            {
                // check if id is in parent class
                if (id.find("FUNC_") == 0)
                {
                    // for example FUNC_6Person_6getAge_v change this to 6getAge_v
                    id = id.substr(5);
                    id = id.substr(id.find("_") + 1);
                    // check if id is in this class as suffix of entry
                    for (auto it : (*table))
                    {
                        std::string key = it.first;
                        if (key.length() >= id.length() &&
                            key.substr(key.length() - id.length()) == id)
                        {
                            id = it.first; // to pass parent class name function to parser.y
                            return (*table)[key];
                        }
                    }
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
        }
        if (class_parent_table.find(temp) == class_parent_table.end())
            break;
        temp = class_parent_table[temp];
    }
    return 0; // class table not found
}

int getClassSize(std::string class_name)
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
int inheritFromClass(std::string childClassName, std::string parentClassName)
{
    // Find the parent class symbol table
    class_sym_table *temp = curr_class_table;
    sym_table *parentClassTable = nullptr;

    // Search for parent class in the class hierarchy
    while (temp)
    {
        if ((*temp).find(parentClassName) != (*temp).end())
        {
            parentClassTable = (*temp)[parentClassName].second;
            break;
        }

        if (class_parent_table.find(temp) == class_parent_table.end())
        {
            break;
        }
        temp = class_parent_table[temp];
    }

    // If parent class not found, report error
    if (!parentClassTable)
    {
        return 0; // Parent class not found
    }

    std::vector<std::pair<std::string, sym_entry *>> inheritableMembers;
    for (auto it : (*parentClassTable))
    {
        std::string memberName = it.first;
        sym_entry *member = it.second;
        inheritableMembers.push_back({memberName, member});
    }

    // Sort members by offset
    std::sort(inheritableMembers.begin(), inheritableMembers.end(),
              [](const auto &a, const auto &b)
              {
                  return a.second->offset < b.second->offset;
              });

    // Insert members in sorted order
    int n = inheritableMembers.size() - 1;
    for (int i = n; i >= 0; i--)
    {
        std::string memberName = inheritableMembers[i].first;
        sym_entry *member = inheritableMembers[i].second;

        // std::cout << "Remooving private Member: " << memberName
                //   << " with offset: " << member->offset << std::endl;
        if (member->access == "private"){
            inheritableMembers.pop_back();
        }
        else break;
            
    }
    for (const auto &pair : inheritableMembers)
    {
        std::string memberName = pair.first;
        sym_entry *member = pair.second;

        // std::cout << "Inside inheritFromClass function Member: " << memberName
                //   << " with offset: " << member->offset << std::endl;
        // offset copy karna padega from parent class to child class
        if ((*curr_class_structure).find(memberName) == (*curr_class_structure).end())
        {
            // Only inherit public and protected members
            if (member->access == "public" || member->access == "protected")
            {
                (*curr_class_structure).insert(std::make_pair(memberName, createEntry(member->type, member->size, member->init, member->offset, nullptr, member->access, member->isStatic, 0, member->isArray, 0, member->array_dims)));
            }
        }
    }
    // add size of last member to class offset
    if (inheritableMembers.size() > 0)
    {
        auto las = inheritableMembers[inheritableMembers.size() - 1].second;
        class_offset +=las->offset + las->size;
        if (!blockSz.empty())
            blockSz.top() += las->offset + las->size;
        if (!Goffset.empty())
            Goffset.top() += las->offset + las->size;
        //TODO: padding 
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
    makeSymbolTable("dummyF_name", "", true);
    avl = 1;
}

void insertSymbol(sym_table &table, std::string id, std::string type, int size, bool is_init, sym_table *ptr, std::string access, int isStatic, bool isConst, bool isArray, bool isEnum, std::vector<int> array_dims)
{
    table.insert(std::make_pair(id, createEntry(type, size, is_init, blockSz.top(), ptr, access, isStatic, isConst, isArray, isEnum, array_dims)));
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

int param_offset = -8;

// insert function parameters into the symbol table of the function
void paramInsert(sym_table &table, std::string id, std::string type, int size, bool is_init, sym_table *ptr, int isStatic, bool isConst, bool isArray, std::vector<int> array_dims)
{
    table.insert(make_pair(id, createEntry(type, size, is_init, param_offset, ptr, "", isStatic, isConst, isArray, 0, array_dims)));

    // if(type[type.length()-1] == '*' && !array_dims.empty()){
    // 	size = 4;
    // 	std::vector<int> temp;
    // 	int curr = 1;
    // 	for(int i = array_dims.size()-1; i>=1; i--){
    // 		curr*=array_dims[i];
    // 		temp.push_back(curr);
    // 	}
    // 	reverse(temp.begin(), temp.end());
    // 	table[id]->array_dims = temp;
    // 	table[id]->offset = param_offset - size;
    // 	array_dims.clear();
    // }
    param_offset -= size;
    if (abs(param_offset) % 4 != 0)
    {
        int padding = (4 - (abs(param_offset) % 4));
        param_offset -= padding;
    }
}

void clear_paramoffset()
{
    param_offset = -8;
}

std::vector<std::string> getFuncArgs(std::string id)
{
    std::vector<std::string> temp;
    temp.push_back("FUNC_DNE");
    if (func_arg.find(id) != func_arg.end())
        return func_arg[id];
    else
        return temp;
}
// Add to symbol_table.cpp
std::string getTypeCode(const std::string &type)
{
    // Convert full type names to single-character codes
    if (type == "int")
        return "i";
    if (type == "float")
        return "f";
    if (type == "char")
        return "c";
    if (type == "void")
        return "v";
    if (type == "bool")
        return "b";
    if (type == "...")
        return "m";
    if (type.find("*") != std::string::npos)
        return "p"; // All pointers
    // Add more types as needed
    return type; // Unknown type
}

std::string mangleFunctionName(const std::string &name, const std::vector<std::string> &paramTypes)
{
    // Check if the function name needs to be mangled (check for printf etc)
    for (auto it : lib_funcs)
    {
        if (name == it)
            return name;
    }

    std::string result = "FUNC_" + std::to_string(name.length()) + name + "_";

    // If no parameters, use 'v' for void
    if (paramTypes.empty())
    {
        result += "v";
        return result;
    }

    // Add type codes for each parameter
    for (const auto &type : paramTypes)
    {
        result += getTypeCode(type);
    }

    return result;
}

void updInit(std::string id)
{
    sym_entry *entry = lookup(id);
    if (entry)
        entry->init = true;
    else
        std::cerr << "Warning: updInit called with unknown id " << id << ".\n";
}

void updTableSize(std::string id)
{
    sym_entry *entry = lookup(id);
    if (entry)
        entry->size = blockSz.top();
    else
        std::cerr << "Warning: updTableSize called with unknown id " << id << ".\n";
}

void insertFuncArg(std::string &func, std::vector<std::string> &arg)
{
    func_arg.insert(std::make_pair(func, arg));
}

void insertType(std::string a, std::string b)
{
    if (curr_typ)
    {
        if ((*curr_typ).find(b) == (*curr_typ).end())
        {
            (*curr_typ).insert(std::make_pair(a, b));
        }
        else
        {
            (*curr_typ).insert(std::make_pair(a, (*curr_typ)[b]));
        }
    }
    else
    {
        std::cerr << "Error: curr_typ is NULL in insertType.\n";
    }
}

std::string lookupType(std::string a)
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

void printSymbolTable(sym_table *table, std::string file_name)
{
    if (!print_symtab)
        return;
    if (!table)
    {
        std::cerr << "Error: NULL table passed to printSymbolTable.\n";
        return;
    }
    std::ofstream outFile(outputDir + file_name);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Cannot open " << file_name << " for writing.\n";
        return;
    }

    // Check if this is a class symbol table (filename starts with "CLASS_")
    bool isClassTable = (file_name.find("CLASS_") == 0);

    // Add the Access column only for class tables
    if (isClassTable)
    {
        outFile << "Name, Type, Size, isInitialized, Offset, Access,isStatic, isConst, isArray, Array Dims\n";
        for (auto it : (*table))
        {
            outFile << it.first << ", "
                    << it.second->type << ", "
                    << it.second->size << ", "
                    << it.second->init << ", "
                    << it.second->offset << ", "
                    << it.second->access << ","
                    << (it.second->isStatic > 0 ? "static" : "") << ", "
                    << (it.second->isConst ? "const" : "") << ", "
                    << (it.second->isArray ? "array" : "");
            // print array dims
            if (it.second->isArray)
            {
                outFile << ",[";
                for (int i = 0; i < it.second->array_dims.size(); i++)
                {
                    outFile << it.second->array_dims[i];
                    if (i != it.second->array_dims.size() - 1)
                        outFile << ", ";
                }
                outFile << "]";
            }
            outFile << "\n";
        }
    }
    else
    {
        // Original format for non-class tables
        outFile << "Name, Type, Size, isInitialized, Offset, isGlobal, isStatic, isConst, isArray, isEnum, Array Dims\n";
        for (auto it : (*table))
        {
            outFile << it.first << ", "
                    << it.second->type << ", "
                    << it.second->size << ", "
                    << it.second->init << ", "
                    << it.second->offset << ", "
                    << (it.second->isGlobal ? "global" : "") << ", "
                    << (it.second->isStatic > 0 ? "static" : "") << ", "
                    << (it.second->isConst ? "const" : "") << ", "
                    << (it.second->isArray ? "array" : "") << ", "
                    << (it.second->isEnum ? "enum const" : "");
            if (it.second->isArray)
            {
                outFile << ",[";
                for (int i = 0; i < it.second->array_dims.size(); i++)
                {
                    outFile << it.second->array_dims[i];
                    if (i != it.second->array_dims.size() - 1)
                        outFile << ", ";
                }
                outFile << "]";
            }
            outFile << "\n";
        }
    }
}

int getSize(std::string id)//need to add support for string literals
{
    if (findStruct(id))
        return getStructsize(id);
    if (findClass(id))
        return getClassSize(id);
    if (id == "int")
    {
        return 4;
    }
    if (id == "va_list")
    {
        return 4;
    }
    if (id == "char")
    {
        return 1;
    }
    return 4; // for any pointer type
}

bool isMatching(std::string var, std::string s2)
{ // s1-> func_im , s2 -> func_iiii
    int n = var.size();
    var.pop_back();
    for (int i = 0; i < var.size(); i++)
    {
        if (s2.size() <= i)
            return false;
        if (var[i] != s2[i])
            return false;
    }
    return true;
}
