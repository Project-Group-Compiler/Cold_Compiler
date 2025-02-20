%{
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "AST.hpp"

extern int yyleng;
std::string currentDataType="";

std::vector<std::pair<std::string,std::string>> typedefTable;

typedef struct {
    std::string token;
    std::string data_type;
} SymbolTableEntry;

char type_of_declarator='n';
int noArgs=0;
bool flag=0;

std::vector<SymbolTableEntry> SymbolTable;
extern std::vector<SymbolTableEntry> ConstantTable;
extern void addToConstantTable(std::string Token, std::string Data_type);

void addToSymbolTable(std::string Token, std::string Data_type) {
    SymbolTableEntry entry;
	entry.token=Token;
	entry.data_type=Data_type;
	SymbolTable.push_back(entry);
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

std::string searchTypedefTable(std::string Token){
	for (int i=0; i<typedefTable.size(); i++){
		if(typedefTable[i].first==Token) return typedefTable[i].second;
	}
	return "";
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

void printType(){
	for(int i=0; i<typedefTable.size(); i++){
		std::cout << typedefTable[i].first << "    " << typedefTable[i].second << '\n';
	}
}

typedef struct token_data
{
	int line;
	int column;
	std::string token_type;
	std::string lexeme;
} TOKEN;


extern char* yytext;
extern int column;
extern int line;
extern std::string inputFilename;
extern bool has_error;

int yyerror(const char*);
int yylex();
%}

%define parse.error detailed
%define parse.lac full

%union{
	char* str;
	int number;
	Node* ptr;
}

%token<str> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token<str> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP INHERITANCE_OP
%token<str> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token<str> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<str> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token<str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token<str> STRUCT UNION ENUM ELLIPSIS
%token<str> CLASS PUBLIC PRIVATE PROTECTED

%token<str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token<str> UNTIL /*** Added UNTIL token ***/

%start translation_unit

%type<ptr> primary_expression postfix_expression argument_expression_list unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression
%type<str> assignment_operator 
%type<ptr> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression 
%type<ptr> assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list
%type<ptr> declarator direct_declarator pointer type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator direct_abstract_declarator initializer
%type<ptr> init_declarator type_specifier struct_or_union_specifier	struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enum_specifier enumerator_list enumerator type_qualifier
%type<ptr> statement labeled_statement compound_statement declaration_list statement_list expression_statement selection_statement iteration_statement jump_statement translation_unit external_declaration function_definition initializer_list
%type<ptr> storage_class_specifier
%type<str> struct_or_union
%type<ptr> class_definition inheritance_specifier inheritance_specifier_list access_specifier class class_definition_head class_internal_definition_list class_internal_definition	class_member_list class_member

%left ';'
%expect 3
%%

primary_expression
    : IDENTIFIER {
    	$$ = createASTNode($1);
		//std::string check=std::string($1);
		//addToSymbolTable(check,currentDataType);
		//std::cout << check << "  " << currentDataType << " added " << "primary_expression" << '\n';
    }
	| CONSTANT 	{
		$$ = createASTNode($1);
	}
	| STRING_LITERAL {
		std::string check=std::string($1);
		addToConstantTable(check,"String Literal");
		//std::cout << check << "  " << currentDataType << " added " << "STRING_LITERAL" << '\n';
		$$ = createASTNode($1);
	}
	| '(' expression ')' {
		$$ = $2;
	}
	;

postfix_expression
	: primary_expression {
		$$ = $1;
	}
	| postfix_expression '[' expression ']' {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
	}
	| postfix_expression '(' ')' {
		$$ = $1;
	}
	| postfix_expression '(' argument_expression_list ')' {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("postfix_expression", &attr);
	}
	| postfix_expression '.' IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode("expression.id", &attr);
	}
	| postfix_expression PTR_OP IDENTIFIER {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, createASTNode($3), "", 1);
		$$ = createASTNode($2, &attr);
	}
	| postfix_expression INC_OP {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| postfix_expression DEC_OP {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

argument_expression_list
	: assignment_expression {
		$$ = $1;
	}
	| argument_expression_list ',' assignment_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("argument_list", &attr);
	}
	;

unary_expression
	: postfix_expression {
		$$ = $1;
	}
	| INC_OP unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| DEC_OP unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| unary_operator cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("unary_exp", &attr);
	}
	| SIZEOF unary_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode($1, &attr);
	}
	| SIZEOF '(' type_name ')' {
		std::vector<Data> attr;
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($1, &attr);
	}
	;

unary_operator
	: '&' {
		$$ = createASTNode("&");
	}
	| '*' {
		$$ = createASTNode("*");
	}
	| '+' {
		$$ = createASTNode("+");
	}
	| '-' {
		$$ = createASTNode("-");
	}
	| '~' {
		$$ = createASTNode("~");
	}
	| '!' {
		$$ = createASTNode("!");
	}
	;

cast_expression
	: unary_expression {
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $2, "", 1);
		insertAttr(attr, $4, "", 1);
		$$ = createASTNode("cast_expression", &attr);
	}
	;

multiplicative_expression
	: cast_expression {
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("*", &attr);
	}
	| multiplicative_expression '/' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("/", &attr);
	}
	| multiplicative_expression '%' cast_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("%", &attr);
	}
	;

additive_expression
	: multiplicative_expression {
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("+", &attr);
	}
	| additive_expression '-' multiplicative_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("-", &attr);
	}
	;

shift_expression
	: additive_expression {
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| shift_expression RIGHT_OP additive_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	; 

relational_expression
	: inclusive_or_expression {
		 $$ = $1;
	}
	| relational_expression '<' inclusive_or_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("<", &attr);
	}
	| relational_expression '>' inclusive_or_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode(">", &attr);
	}
	| relational_expression LE_OP inclusive_or_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| relational_expression GE_OP inclusive_or_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

equality_expression
	: relational_expression { 
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	| equality_expression NE_OP relational_expression {
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

and_expression
	: shift_expression {	
		$$ = $1; 
	}
	| and_expression '&' shift_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&", &attr);
	}
	;

exclusive_or_expression
	: and_expression {
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("^", &attr);
    }
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("|", &attr);
	}
	;

logical_and_expression
	: equality_expression {$$ = $1;}
	| logical_and_expression AND_OP equality_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("&&", &attr);
	}
	;

logical_or_expression
	: logical_and_expression{$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("||", &attr);
	}
	;

conditional_expression
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		insertAttr(attr, $5, "", 1);
		$$ = createASTNode("ternary operator", &attr);
	}
	;

assignment_expression
	: conditional_expression{$$ = $1;}
	| unary_expression assignment_operator assignment_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode($2, &attr);
	}
	;

assignment_operator
	: '='	{
		$$ = strdup("=");
	}
	| MUL_ASSIGN		{$$ = $1;}
	| DIV_ASSIGN		{$$ = $1;}
	| MOD_ASSIGN		{$$ = $1;}
	| ADD_ASSIGN		{$$ = $1;}
	| SUB_ASSIGN		{$$ = $1;}
	| LEFT_ASSIGN		{$$ = $1;}
	| RIGHT_ASSIGN		{$$ = $1;}
	| AND_ASSIGN		{$$ = $1;}
	| XOR_ASSIGN		{$$ = $1;}
	| OR_ASSIGN			{$$ = $1;}
	;

expression
	: assignment_expression{$$ = $1;}
	| expression ',' assignment_expression{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("expression", &attr);
	}
	;

constant_expression
	: conditional_expression{$$ = $1;}
	;

declaration
	: declaration_specifiers ';'{ $$ = $1; }
	| declaration_specifiers init_declarator_list ';'{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration", &attr);
	}
	;

declaration_specifiers
	: storage_class_specifier { $$ = $1; }
	| storage_class_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_specifier{ $$ = $1; }
	| type_specifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	| type_qualifier{ $$ = $1; }
	| type_qualifier declaration_specifiers{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $2, "", 1);
		$$ = createASTNode("declaration_specifiers", &attr);
	}
	;

init_declarator_list
	: init_declarator{$$ = $1;}
	| init_declarator_list ',' init_declarator{
		std::vector<Data> attr;
		insertAttr(attr, $1, "", 1);
		insertAttr(attr, $3, "", 1);
		$$ = createASTNode("init_declarator_list", &attr);
	}
	;

init_declarator
	: declarator {$$ = $1;}
	| declarator '=' initializer{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

storage_class_specifier
	: TYPEDEF	{ $$ = createASTNode($1); flag=1;}
	| EXTERN	{ $$ = createASTNode($1);}
	| STATIC	{ $$ = createASTNode($1);}
	| AUTO		{ $$ = createASTNode($1);}
	| REGISTER	{ $$ = createASTNode($1);}
	;

type_specifier
	: VOID			{$$ = createASTNode($1); currentDataType="void"; type_of_declarator='d';}	
	| CHAR			{$$ = createASTNode($1); currentDataType="char"; type_of_declarator='d';}	
	| SHORT			{$$ = createASTNode($1); currentDataType="short"; type_of_declarator='d';}	
	| INT			{$$ = createASTNode($1); currentDataType="int"; type_of_declarator='d';}
	| LONG			{$$ = createASTNode($1); currentDataType="long"; type_of_declarator='d';}
	| FLOAT			{$$ = createASTNode($1); currentDataType="float"; type_of_declarator='d';}
	| DOUBLE		{$$ = createASTNode($1); currentDataType="double"; type_of_declarator='d';}
	| SIGNED		{$$ = createASTNode($1); currentDataType="signed"; type_of_declarator='d';}
	| UNSIGNED		{$$ = createASTNode($1); currentDataType="unsigned"; type_of_declarator='d';}
	| struct_or_union_specifier	{$$ = $1;}	
	| class_definition 			{$$ = $1;}
	| enum_specifier			{$$ = $1;}
	| TYPE_NAME		{$$ = createASTNode($1);}	
	;

inheritance_specifier
	: access_specifier IDENTIFIER {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        /* Wrap IDENTIFIER into an AST node */
        insertAttr(attr, createASTNode($2), "", 1);
        $$ = createASTNode("inheritance_specifier", &attr);
    }
	;

inheritance_specifier_list
	: inheritance_specifier { $$ = $1; }
	| inheritance_specifier_list ',' inheritance_specifier{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("inheritance_specifier_list", &attr);
    }
	;

access_specifier 
	: PRIVATE { $$ = createASTNode($1); }
	| PUBLIC { $$ = createASTNode($1); }
	| PROTECTED { $$ = createASTNode($1); }
	;

class
	: CLASS { $$ = createASTNode($1); }
	;

class_definition_head 
	: class INHERITANCE_OP inheritance_specifier_list{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_definition_head", &attr);
    }
	| class IDENTIFIER {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        $$ = createASTNode("class_definition_head", &attr);
    }
	| class IDENTIFIER  INHERITANCE_OP inheritance_specifier_list{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($2), "", 1);
        insertAttr(attr, $4, "", 1);
        $$ = createASTNode("class_definition_head", &attr);
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}'{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_definition", &attr);
    }
	| class_definition_head { $$ = $1; }
	;

class_internal_definition_list
	: class_internal_definition { $$ = $1; }
	| class_internal_definition_list class_internal_definition {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_internal_definition_list", &attr);
    }
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';'{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("class_internal_definition", &attr);
    }
	;

class_member_list
	: class_member{ $$ = $1; }
	| class_member_list class_member{
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("class_member_list", &attr);
    }
	;

class_member
	: function_definition { $$ = $1; }
	| declaration { $$ = $1; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		std::string check=std::string($2);
		//addToSymbolTable(check,currentDataType);
		$$ = createASTNode($1, &v);
	}
	| struct_or_union '{' struct_declaration_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
	}
	| struct_or_union IDENTIFIER 	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		std::string check=std::string($2);
		currentDataType+=" ";
		currentDataType+=check;
		$$ = createASTNode($1, &v);
	}
	;

struct_or_union
	: STRUCT	{$$ = $1; currentDataType="struct";}
	| UNION		{$$ = $1;}
	;

struct_declaration_list
	: struct_declaration	{ $$ = $1 ;}
	| struct_declaration_list struct_declaration 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration_list", &v);
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("struct_declaration", &v);
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("specifier_qualifier_list", &v);
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("struct_declarator_list", &v);
	}
	;

struct_declarator
	: declarator	{ $$ = $1; }
	| ':' constant_expression	{ $$ = $2; }
	| declarator ':' constant_expression	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode(":", &v);
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode($1, &v);
	}
	| ENUM IDENTIFIER {
		std::vector<Data> v;
		insertAttr(v, createASTNode($2), "", 1);
		$$ = createASTNode($1, &v);
	}
	;

enumerator_list
	: enumerator 	{ $$ = $1; }
	| enumerator_list ',' enumerator 	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("enumerator_list", &v);
	}
	;

enumerator
	: IDENTIFIER	{ $$ = createASTNode($1); }
	| IDENTIFIER '=' constant_expression 	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("=", &v);
	}
	;

type_qualifier
	: CONST		{ $$ = createASTNode($1); }
	| VOLATILE	{ $$ = createASTNode($1); }
	;


declarator
	: pointer direct_declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declarator", &v);
	}
	| direct_declarator {
		$$ = $1 ;
	}
	;


direct_declarator
	: IDENTIFIER {
		$$ = createASTNode($1);
		std::string check=std::string($1);
		if(flag){
			typedefTable.push_back(make_pair(check,currentDataType));
			flag=0;
		}
		else{
		std::string check=std::string($1);
		addToSymbolTable(check,currentDataType);
		//std::cout << check << "  " << currentDataType << " added " << "direct_declarator for primitives" << '\n';
		type_of_declarator='n';}
	}
	| '(' declarator ')'  {
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		updateLastSymbolEntry();
	}
	| direct_declarator '[' ']'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "[ ]", 0);
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		//std::cout << SymbolTable.back().token << "      " << SymbolTable.back().data_type << '\n'; 
		updateFuncSymbolEntry(noArgs);
		noArgs=0;
	}
	| direct_declarator '(' identifier_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
		//updateFuncSymbolEntry();
	}
	| direct_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, NULL, "( )", 0);
		$$ = createASTNode("direct_declarator", &v);
		updateFuncSymbolEntry(0);
	}
	;

pointer
	: '*' {
		currentDataType+="*";
		std:: cout << currentDataType << '\n';
		$$ = createASTNode("*(Pointer)");
	}
	| '*' type_qualifier_list{
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	| '*' pointer{
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	| '*' type_qualifier_list pointer{
		currentDataType+="*";
		std::vector<Data> v;
		insertAttr(v,$2,"",1);
		insertAttr(v,$3,"",1);
		$$ = createASTNode("*(Pointer)",&v);
	}
	;

type_qualifier_list
	: type_qualifier {
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_qualifier_list",&v);
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v, createASTNode($3), "", 1);
		$$ = createASTNode("parameter_type_list",&v);
	}
	;

parameter_list
	: parameter_declaration{
		//std::cout << SymbolTable.back().token << "   "  << " single   " << SymbolTable.back().data_type << "  " << noArgs << '\n';
		//updateFuncSymbolEntry(noArgs);
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		//std::cout << SymbolTable.back().token << " " << "list  " << SymbolTable.back().data_type << "  " << noArgs << '\n';
		noArgs++;
		$$ = createASTNode("parameter_list",&v);
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
	}
	| declaration_specifiers abstract_declarator{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("parameter_declaration",&v);
	}
	| declaration_specifiers {
		$$ = $1;
	}
	;

identifier_list
	: IDENTIFIER {
		$$ =createASTNode($1);
	}
	| identifier_list ',' IDENTIFIER{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,createASTNode($3),"",1);
		$$ = createASTNode("identifier_list",&v);
	}
	;

type_name
	: specifier_qualifier_list{
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("type_name",&v);
	}
	;

abstract_declarator
	: pointer {
		$$ =$1;
	}
	| direct_abstract_declarator{
		$$ = $1;
	}
	| pointer direct_abstract_declarator{
		std::vector<Data> v;
		insertAttr(v,$1,"",1);
		insertAttr(v,$2,"",1);
		$$ = createASTNode("abstract_declarator",&v);
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = createASTNode("[ ]") ;
	}
	| '[' constant_expression ']' {
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {
		std::vector<Data> v;
		insertAttr(v,NULL,"[ ]",0);
		insertAttr(v,$1,"",1);
		$$ = createASTNode("direct_abstract_declarator",&v);
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, NULL, 1);
		Node* node = createASTNode("[ ]", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
	}
	| '(' ')'{
		$$ = createASTNode("( )") ;
	}
	| '(' parameter_type_list ')'{
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{
		std::vector<Data> v;
		insertAttr(v, NULL, "( )", 0);
		insertAttr(v, $1, "", 1);
		$$ = createASTNode("direct_abstract_declarator",&v);
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_abstract_declarator", &v);
	}
	;

initializer
	: assignment_expression{
		$$ = $1 ;
	}
	| '{' initializer_list '}' {
		$$ = $2 ;
	}
	| '{' initializer_list ',' '}'{
		$$ = $2;
	}
	;


initializer_list
	: initializer	{
		$$ = $1;
	}
	| initializer_list ',' initializer	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("initializer_list", &v);
	}
	;

statement
	: labeled_statement	{$$ = $1;}
	| compound_statement	{$$ = $1;}
	| expression_statement	{$$ = $1;}
	| selection_statement	{$$ = $1;}
	| iteration_statement	{$$ = $1;}
	| jump_statement	{$$ = $1;}
	| error ';' {$$ = new Node; yyclearin; yyerrok;}
	;

labeled_statement
	: IDENTIFIER ':' statement	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("labeled_statement", &v);
	}
	| CASE constant_expression ':' statement	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("case", &v);
	}
	| DEFAULT ':' statement	{
		std::vector<Data> v;
		insertAttr(v, NULL, "default", 0);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("case", &v);
	}
	;

compound_statement
	: '{' '}'	{$$ = createASTNode("{ }");}
	| '{' statement_list '}'	{$$ = $2;}
	| '{' declaration_list '}'	{$$ = $2;}
	| '{' declaration_list statement_list '}'	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("compound_statement", &v);
	}
	;

declaration_list
	: declaration	{$$ = $1;}
	| declaration_list declaration	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("declaration_list", &v);
	}
	;

statement_list
	: statement	{$$ = $1;}
	| statement_list statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("statement_list", &v);
	}
	;

expression_statement
	: ';'	{$$ = createASTNode(";");}
	| expression ';'	{$$ = $1;}
	;

selection_statement
	: IF '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("if", &v);
	}
	| IF '(' expression ')' statement ELSE statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("if-else", &v);
	}
	| SWITCH '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("switch", &v);
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("while-loop", &v);
	}
	| DO statement WHILE '(' expression ')' ';'	{
		std::vector<Data> v;
		insertAttr(v, $2, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("do-while-loop", &v);
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $6, "", 1);
		$$ = createASTNode("for-loop(w/o update stmt)", &v);
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		insertAttr(v, $5, "", 1);
		insertAttr(v, $7, "", 1);
		$$ = createASTNode("for-loop", &v);
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		std::vector<Data> v;
		insertAttr(v, $3, "", 1);
		insertAttr(v, $5, "", 1);
		$$ = createASTNode("until-loop", &v);
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
		std::string s;
		s = (std::string)$1 + " : " + (std::string)$2;
        $$ = createASTNode(s);
	}
	| CONTINUE ';'	{$$ = createASTNode($1);}
	| BREAK ';'		{$$ = createASTNode($1);}
	| RETURN ';'	{$$ = createASTNode($1);}
	| RETURN expression ';'	{
		std::vector<Data> v;
		insertAttr(v, createASTNode($1), "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("jump_stmt", &v);
	}
	;

translation_unit
	: external_declaration	{
		$$ = $1;
	}
	| translation_unit external_declaration	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("program", &v);
	}
	;

external_declaration
	: function_definition	{ $$ = $1;}
	| declaration	{ $$ = $1;}
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		insertAttr(v, $4, "", 1);
		$$ = createASTNode("function", &v);
	}
	| declaration_specifiers declarator compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		insertAttr(v, $3, "", 1);
		$$ = createASTNode("function (w/o decl_list)", &v);
	}
	| declarator declaration_list compound_statement	{
		std::vector<Data> v;
                insertAttr(v, $1, "", 1);
                insertAttr(v, $2, "", 1);
                insertAttr(v, $3, "", 1);
                $$ = createASTNode("function (w/o decl_specifiers)", &v);
	}
	| declarator compound_statement	{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $2, "", 1);
		$$ = createASTNode("function (w/o specifiers and decl_list)", &v);
	}
	;

%%

void performParsing(const std::string &inputFile)
{
    beginAST(inputFile);
    yyparse();
    endAST();

    fclose(dotfile);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        print_error("No input files provided.");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l") { // Lexer-only mode
            if (i + 1 >= argc) {
                print_error("Missing filename for -l option.");
                return -1;
            }
            onlyLexer = true;
            lexerOutputFile = argv[++i];
            continue;
        }

		currentFilename = argv[i];
        yyin = fopen(argv[i], "r");
        if (!yyin) {
            print_error("Cannot open file " + arg);
            continue;
        }

        line = 1;
        column = 0;
        yyrestart(yyin);

        if (onlyLexer) {
            performLexicalAnalysis(arg);
        } else {
            performParsing();
        }

        fclose(yyin);
    }
	printSymbolTable();
	printConstantTable();
	printType();
    return 0;
}

// Error handling function
int yyerror(const char* s) 
{
	has_error = true;
    std::ifstream file(inputFilename);  
    std::string curr_line;
    int count = 1;
	std::string heading("syntax error");
	std::string error_line(s);
	int pos = error_line.find_first_of(heading);
	if (pos != std::string::npos)
		error_line.erase(pos, heading.length() + 2);

    while (std::getline(file, curr_line)) 
	{
        if (count == line) {
            std::cerr << "\033[1;31merror: \033[0m" << heading << "::" << line << ":" << column - yyleng << ": " << error_line << "\n\n";
            std::cerr << line << " | " << curr_line << "\n";
            std::cerr << std::string(column - yyleng + 4, ' ') << "^\n";
            break;
        }
        count++;
    }
    file.close();
    return -1;
}
