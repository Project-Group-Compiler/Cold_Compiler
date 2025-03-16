#include "symbol_table.h"

sym_table gst;
struct_sym_table struct_gst;
map<sym_table*, sym_table*> parent_table;
map<struct_sym_table*, struct_sym_table*> struct_parent_table;

map<string, ull> struct_size;
map<string, vector<string> > func_arg;
ull struct_offset;
sym_table* curr_table; //store pointer of the current symbol table
sym_table* curr_structure;
struct_sym_table *curr_struct_table;
stack<ull> Goffset, Loffset, blockSz;

typ_table typ_gst;  //map<string, string> typ_table;
map<typ_table*, typ_table*> typ_parent_table;
typ_table* curr_typ;


int struct_count = 1;
int avl=0;

int blockCnt = 1;

void symTable_init(){
	Goffset.push(0);
	Loffset.push(0);
	blockSz.push(0);
	parent_table.insert(make_pair(&gst, nullptr));
	struct_parent_table.insert(make_pair(&struct_gst, nullptr));
	curr_table = &gst;
	curr_struct_table = &struct_gst;
	curr_typ = &typ_gst;
	insertKeywords();
}


sym_entry* createEntry(string type, ull size, bool init, ull offset, sym_table* ptr){
	sym_entry* new_sym = new sym_entry();
	new_sym->type = type;
	new_sym->size = size;
	new_sym->init = init;
	new_sym->offset = offset;
	new_sym->entry = ptr;
	return new_sym;
}

void makeSymbolTable(string name, string f_type){
	if(!avl){
		sym_table* new_table = new sym_table;
		struct_sym_table* new_struct_table = new struct_sym_table;
		typ_table* new_typ = new typ_table;

		if(f_type != "") insertSymbol(*curr_table, name , "FUNC_" + f_type , 0 , 1, new_table);
		else{
			insertSymbol(*curr_table, name , "Block",0,1, new_table);
			blockCnt++;
		}

		Goffset.push(0);
		blockSz.push(0);
		parent_table.insert(make_pair(new_table, curr_table));
		struct_parent_table.insert(make_pair(new_struct_table, curr_struct_table));
		typ_parent_table.insert(make_pair(new_typ, curr_typ));

		curr_table = new_table;
		curr_struct_table = new_struct_table;
		curr_typ = new_typ;

	}
	else{
		avl = 0;
		(*parent_table[curr_table]).erase("dummyF_name");
		(*parent_table[curr_table]).insert(make_pair(name,createEntry("FUNC_"+f_type,0,1,Loffset.top(), curr_table)));
		Loffset.pop();
	}
}

void removeFuncProto(){
	avl = 0;
	updSymbolTable("dummyF_name");
	parent_table.erase((*curr_table)["dummyF_name"]->entry);
	(*curr_table).erase("dummyF_name");
	Loffset.pop();
}

void updSymbolTable(string id){
	ull temp = Goffset.top();
	Goffset.pop();
	Goffset.top()+=temp;

	curr_table = parent_table[curr_table];
	curr_struct_table = struct_parent_table[curr_struct_table];
	curr_typ = typ_parent_table[curr_typ];

	sym_entry* entry = lookup(id);
	if(entry) entry->size = blockSz.top();

	temp = blockSz.top();
	blockSz.pop();
	blockSz.top()+=temp;

}

sym_entry* lookup(string id){
	sym_table* temp = curr_table;

	while(temp){
		if((*temp).find(id)!=(*temp).end()) return (*temp)[id];
		temp = parent_table[temp];
	}
	return nullptr;
}

sym_entry* currLookup(string id){
	if((*curr_table).find(id)==(*curr_table).end()) return nullptr;
	return (*curr_table)[id];
}

void insertKeywords(){
	vector<string> key_words = {"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"}; 
	vector<string> op = {"...",">>=","<<=","+=","-=","*=","/=","%=","&=","^=","|=",">>","<<","++","--","->","&&","||","<=",">=","==","!=",";","{","<%","}","%>",",",":","=","(",")","[","<:","]",":>",".","&","!","~","-","+","*","/","%","<",">","^","|","?"};

	for(auto h:key_words){
		insertSymbol(*curr_table, h, "keyword", 8, 1, nullptr);
	}
	for(auto h:op){
		insertSymbol(*curr_table, h, "operator", 8, 1, nullptr);
	}
}

string getType(string id){
	sym_entry* entry = lookup(id);
	string ret = "";
	if(entry) ret += entry->type;
	
	return ret;
}


void createStructTable(){
	sym_table* new_table = new sym_table;
	curr_structure = new_table;
	struct_offset = 0;
}

// insert struct attributes in struct tsymbol table
int insertStructAttr(string attr, string type, ull size, bool init){  
	if((*curr_structure).find(attr)==(*curr_structure).end()){
		blockSz.top()+=size;
		Goffset.top()+=size;
		(*curr_structure).insert(make_pair(attr,createEntry(type,size,init, struct_offset, nullptr)));
		struct_offset += size;
		return 1;
	}
	return 0;
}

int printStructTable(string struct_name){
	if((*curr_struct_table).find(struct_name)==(*curr_struct_table).end()){
		//struct_parent_table.insert(make_pair(curr_structure, nullptr));
		(*curr_struct_table).insert(make_pair(struct_name, make_pair(struct_offset,curr_structure)));
		//struct_size.insert(make_pair(struct_name, struct_offset)); //TODO
		printSymbolTable(curr_structure, struct_name+ "_" + to_string(struct_count)+".csv");  // prints structre symbol table
		struct_count++;
		return 1;
	}
	return 0;
}

string StructAttrType(string struct_name, string id){
	struct_sym_table* temp = curr_struct_table;
	while((*temp).find(struct_name) == (*temp).end()){
		temp = struct_parent_table[temp];
	}

	sym_table* table = (*temp)[struct_name].second;
	// todo negation
	return ((*table)[id]->type);
}

int findStruct(string struct_name){

	struct_sym_table* temp = curr_struct_table;

	while(temp){
		if((*temp).find(struct_name)!=(*temp).end()) return 1;
		temp = struct_parent_table[temp];
	}
	return 0;

	//return (struct_sym_table.find(struct_name)!=struct_sym_table.end());
}

int lookupStruct(string struct_name, string id){

	struct_sym_table* temp = curr_struct_table;

	while(temp){
		if((*temp).find(struct_name)!=(*temp).end()){
			if((*((*temp)[struct_name].second)).find(id)!=(*((*temp)[struct_name].second)).end()) return 1; // found
			else return 0; // struct doesnt contain attr id
		}
		temp = struct_parent_table[temp];
	}
	return -1;// struct table not found

	// if(struct_sym_table.find(struct_name)!=struct_sym_table.end()){
	// 	if((*struct_sym_table[struct_name]).find(id)!=(*struct_sym_table[struct_name]).end()) return 1;
	// 	else return 0; // struct doesnt contain attr id
	// }
	
}

ull getStructsize(string struct_name){
	struct_sym_table* temp = curr_struct_table;

	while(temp){
		if((*temp).find(struct_name)!=(*temp).end()){
			return (*temp)[struct_name].first;
		}
		temp = struct_parent_table[temp];
	}
	return 0;
}

void createParamList(){
	Loffset.push(Goffset.top());
	makeSymbolTable("dummyF_name", "");
	avl = 1;
}

void insertSymbol(sym_table& table, string id, string type, ull size, bool is_init, sym_table* ptr){
	table.insert(make_pair(id, createEntry(type, size, is_init, Goffset.top(), ptr)));
	blockSz.top()+=size;
	Goffset.top()+=size;
}

vector<string> getFuncArgs(string id){
	vector<string> temp;
	temp.push_back("#NO_FUNC");
	if(func_arg.find(id) != func_arg.end()) return func_arg[id];
	else return temp;
}

void updInit(string id){
	sym_entry* entry = lookup(id);
	if(entry) entry->init = true;
}

void updTableSize(string id){
	sym_entry* entry = lookup(id);
	if(entry) entry->size = blockSz.top();
}

void insertFuncArg(string &func, vector<string> &arg){
	func_arg.insert(make_pair(func, arg));
}

void insertType(string a, string b){
	if((*curr_typ).find(b)==(*curr_typ).end()){
		(*curr_typ).insert(make_pair(a,b));
	}
	else{
		(*curr_typ).insert(make_pair(a, (*curr_typ)[b]));
	}
}

string lookupType(string a){
	typ_table* temp = curr_typ;

	while(temp){
		if((*temp).find(a)==(*temp).end()) return (*temp)[a];
		temp = typ_parent_table[temp];
	}

	return "";
}

void printFuncArg(){
	FILE* file = fopen("FuncArg.csv","w");
    for(auto it:func_arg){

    	string temp = "";
    	for(auto h:it.second){
    		if(temp!="") temp += ", ";
    		temp+=h;
    	}
        fprintf(file,"%s, %s\n",it.first.c_str(),temp.c_str());
     }
     fclose(file);
}
void printSymbolTable(sym_table* table, string file_name){
	FILE* file = fopen(file_name.c_str(), "w");
  	fprintf( file,"Name, Type, Size, isInitialized, Offset\n");
  	for(auto it: (*table)){
    	fprintf(file,"%s,%s,", it.first.c_str(), it.second->type.c_str());
		fprintf(file, "%lld,%d,%lld\n", (it.second)->size, (it.second)->init, (it.second)->offset);
  }
  fclose(file);
}

ull getSize(string id){
  if(struct_size.find(id)!=struct_size.end()) return getStructsize(id);
  if(id == "char") return sizeof(char);
  if(id == "short") return sizeof(short);
  if(id == "short int") return sizeof(short int);
  if(id == "int") return sizeof(int);
  if(id == "long int") return sizeof(long int);
  if(id == "long long") return sizeof(long long);
  if(id == "long long int") return sizeof(long long int);
  if(id == "float") return sizeof(float);
  if(id == "double") return sizeof(double);
  if(id == "long double") return sizeof(long double);
  if(id == "signed short int") return sizeof(signed short int);
  if(id == "signed int") return sizeof(signed int);
  if(id == "signed long int") return sizeof(signed long int);
  if(id == "signed long long") return sizeof(signed long long);
  if(id == "signed long long int") return sizeof(signed long long int);
  if(id == "unsigned short int") return sizeof(unsigned short int);
  if(id == "unsigned int") return sizeof(unsigned int);
  if(id == "unsigned long int") return sizeof(unsigned long int);
  if(id == "unsigned long long") return sizeof(unsigned long long);
  if(id == "unsigned long long int") return sizeof(unsigned long long int);

  return 8; // for any ptr
}
