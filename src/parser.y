%{
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstring>
#include "AST.hpp"
#include "tac.hpp"
#include "data_structures.hpp"

extern int yyleng;
std::string currentDataType="";

int noArgs=0;
bool flag=0,flag2=0;;

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
%token<str> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID FILE_MAN
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
%%

primary_expression
    : IDENTIFIER {
        $$ = createASTNode($1);

        // -- 3AC Generation
        SymbolTableEntry* entry = lookup($1);
		
        if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
            if(entry->data_type == "Procedure") {
                $$->place = qid("CALL " + std::string($1), entry);
            } 
            else {
                $$->place = qid(std::string($1), entry);
            }
        } 
        else {
            $$->place = qid("UNKNOWN", nullptr);
        }
    }
    | CONSTANT {
        $$ = createASTNode($1);

        // -- 3AC Generation
		SymbolTableEntry* entry = lookup($1);
		if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
			$$->place = qid(entry->token, entry);
		}else {
            $$->place = qid("UNKNOWN", nullptr);
        }
    }
    | STRING_LITERAL {
        addToConstantTable(std::string($1), "String Literal");
        $$ = createASTNode($1);

        // -- 3AC Generation
        SymbolTableEntry* entry = lookup($1);
		if(entry) {
			$$->type = entry->data_type;
			$$->temp_name = entry->token;
			$$->nextlist.clear();
			$$->place = qid(entry->token, entry);
		}else {
            $$->place = qid("UNKNOWN", nullptr);
        }
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

        //-- 3AC Generation
		$$->type = $1->type;
		$$->temp_name = $1->temp_name;
		qid temp_var = getTempVariable($$->type);
		$$->place = temp_var;
		$$->nextlist.clear();
		emit(qid("[ ]", NULL), $1->place, $3->place, temp_var, -1);
    }
    | postfix_expression '(' ')' {
        $$ = $1;

        //-- 3AC Generation
		// Todo -> $$->type
		qid q = getTempVariable($1->type);
		$$->place = q;
		$$->temp_name = $1->temp_name;
		$$->nextlist.clear();
		emit(qid("CALL", NULL), qid($$->temp_name, NULL), qid("0", NULL), q, -1);
    }
    | postfix_expression '(' argument_expression_list ')' {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("postfix_expression", &attr);

        //-- 3AC Generation
		// Todo -> $$->type
		qid q = getTempVariable($1->type);
		$$->place = q;
		$$->temp_name = $1->temp_name;
		$$->nextlist.clear();
		emit(qid("CALL", NULL), qid($$->temp_name, NULL), qid(std::to_string($3->argCount), NULL), q, -1);
    }
    | postfix_expression '.' IDENTIFIER { //p.x
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($3), "", 1);
        $$ = createASTNode("expression.id", &attr);

        //-- 3AC Generation
		// Todo -> $$->type
        qid temp_var = getTempVariable($$->type);
        $$->place = temp_var;
		// $$->nextlist.clear();
        emit(qid("member_access", NULL), $1->place, qid(std::string($3), lookup($3)), temp_var, -1);
    }
    | postfix_expression PTR_OP IDENTIFIER {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, createASTNode($3), "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
		// Todo $$ -> type
		qid temp_var = getTempVariable($$->type);
		emit(qid("PTR_OP", NULL), $1->place, qid(std::string($3), lookup($3)), temp_var, -1);
		$$->place = temp_var;
    }
    | postfix_expression INC_OP {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable($1->type);
        $$->place = q;
		$$->type = $1->type;
		$$->nextlist.clear();
        emit(qid("S++", NULL), $1->place, qid("", NULL), q, -1);
    }
    | postfix_expression DEC_OP {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable($1->type);
        $$->place = q;
		$$->type = $1->type;
		$$->nextlist.clear();
        emit(qid("S--", NULL), $1->place, qid("", NULL), q, -1);
    }
;


argument_expression_list
    : assignment_expression {
        $$ = $1;

        //-- 3AC Generation
        $$->argCount = 1;
		$$->nextlist.clear();
        emit(qid("param", NULL), $$->place, qid("", NULL), qid("", NULL), -1);
    }
    | argument_expression_list ',' assignment_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("argument_list", &attr);

        //-- 3AC Generation
		$$->nextlist.clear();
        $$->argCount = $1->argCount + 1;
        int Label = emit(qid("param", NULL), $3->place, qid("", NULL), qid("", NULL), -1);
		backpatch($1->nextlist, Label);
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

        //-- 3AC Generation
        qid q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
        $$->nextlist.clear();
        emit(qid("++P", NULL), $2->place, qid("", NULL), q, -1);
    }
    | DEC_OP unary_expression {
        std::vector<Data> attr;
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode($1, &attr);

        //-- 3AC Generation
        qid q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
        $$->nextlist.clear();
        emit(qid("--P", NULL), $2->place, qid("", NULL), q, -1);
    }
    | unary_operator cast_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode("unary_exp", &attr);

        //-- 3AC Generation
        qid q = getTempVariable($2->type);
		$$->place = q;
		$$->type = $2->type; //Todo not always
		$$->temp_name = $2->temp_name;
        $$->place = q;
        $$->nextlist.clear();
        emit($1->place, $2->place, qid("", NULL), q, -1);
    }
    | SIZEOF unary_expression {
        std::vector<Data> attr;
        insertAttr(attr, $2, "", 1);
        $$ = createASTNode($1, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
		$$->type = "int";
        $$->place = q;
        $$->nextlist.clear();
        emit(qid("SIZEOF", NULL), $2->place, qid("", NULL), q, -1);
    }
    | SIZEOF '(' type_name ')' {
        std::vector<Data> attr;
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode($1, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
        $$->place = q;
		$$->type = "int";
        $$->nextlist.clear();

        emit(qid("SIZEOF", NULL), $3->place, qid("", NULL), q, -1);
    }
;

unary_operator
    : '&' {
        $$ = createASTNode("&");

        //-- 3AC Generation
        $$->place = qid("unary&", lookup("&"));
    }
    | '*' {
        $$ = createASTNode("*");

        //-- 3AC Generation
        $$->place = qid("unary*", lookup("*"));
    }
    | '+' {
        $$ = createASTNode("+");

        //-- 3AC Generation
        $$->place = qid("unary+", lookup("+"));
    }
    | '-' {
        $$ = createASTNode("-");

        //-- 3AC Generation
        $$->place = qid("unary-", lookup("-"));
    }
    | '~' {
        $$ = createASTNode("~");

        //-- 3AC Generation
        $$->place = qid("~", lookup("~"));
    }
    | '!' {
        $$ = createASTNode("!");

        //-- 3AC Generation
        $$->place = qid("!", lookup("!"));
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

        //-- 3AC Generation
        qid q = getTempVariable($2->type);
        $$->place = q;
		$$->type = $2->type;
		$4->nextlist.clear();
		//TODO: Try to do CAST_typename
        emit(qid("CAST", NULL), $4->place, qid("", NULL), q, -1);
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

        //-- 3AC Generation
        qid q = getTempVariable("int"); //TODO not always int
		$$->type = "int"; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit(qid("*", NULL), $1->place, $3->place, q, -1);
    }
    | multiplicative_expression '/' cast_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("/", &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int"); //TODO not always int
		$$->type = "int"; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit(qid("/", NULL), $1->place, $3->place, q, -1);
    }
    | multiplicative_expression '%' cast_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("%", &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int"); //TODO not always int
		$$->type = "int"; //TODO not always int
        $$->place = q;
        $$->nextlist.clear();
        emit(qid("%", NULL), $1->place, $3->place, q, -1);
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

        //-- 3AC Generation
        qid q = getTempVariable("int");//TODO not always int
		$$->type = "int"; // TODO not always int
        $$->place = q;
        $$->nextlist.clear();

        emit(qid("+", NULL), $1->place, $3->place, q, -1);
    }
    | additive_expression '-' multiplicative_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("-", &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");//TODO not always int
		$$->type = "int"; // TODO not always int
        $$->place = q;
        $$->nextlist.clear();

        emit(qid("-", NULL), $1->place, $3->place, q, -1);
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

        //-- 3AC Generation
        qid q = getTempVariable("int");//TODO not always int
		$$->type = "int"; 
        $$->place = q;
        $$->nextlist.clear();

        emit(qid("<<", NULL), $1->place, $3->place, q, -1);
    }
    | shift_expression RIGHT_OP additive_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");//TODO not always int
		$$->type = "int"; 
        $$->place = q;
        $$->nextlist.clear();

        emit(qid(">>", NULL), $1->place, $3->place, q, -1);
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

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid("<", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression '>' inclusive_or_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode(">", &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid(">", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression LE_OP inclusive_or_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid("<=", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | relational_expression GE_OP inclusive_or_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid(">=", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
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

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid("==", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
    | equality_expression NE_OP relational_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode($2, &attr);

        //-- 3AC Generation
        qid q = getTempVariable("int");
        emit(qid("!=", NULL), $1->place, $3->place, q, -1);
        $$->place = q;
        $$->nextlist.clear();
    }
;

and_expression
    : shift_expression {	
        $$ = $1; 
    }
    | and_expression '&' shift_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("&", &attr);

        // --- 3AC Generation ---
		qid temp = getTempVariable("int");  
		emit(qid("&", NULL), $1->place, $3->place, temp, -1);
		$$->place = temp;
        $$->nextlist.clear();
    }
;


exclusive_or_expression
    : and_expression {
        $$ = $1;
    }
    | exclusive_or_expression '^' and_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("^", &attr);

        // --- 3AC Generation ---
        qid temp = getTempVariable("int");  
        emit(qid("^", NULL), $1->place, $3->place, temp, -1);
        $$->place = temp;

        $$->nextlist.clear();
    }
;


inclusive_or_expression
    : exclusive_or_expression { 
        $$ = $1; 
    }
    | inclusive_or_expression '|' exclusive_or_expression {
        std::vector<Data> attr;
        insertAttr(attr, $1, "", 1);
        insertAttr(attr, $3, "", 1);
        $$ = createASTNode("|", &attr);

        // --- 3AC Generation ---
        qid temp = getTempVariable("int"); 
        emit(qid("|", NULL), $1->place, $3->place, temp, -1);
        $$->place = temp;

        $$->nextlist.clear();
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
	| EXTERN	{ $$ = createASTNode($1); currentDataType="extern "; flag2=1;}
	| STATIC	{ $$ = createASTNode($1); currentDataType="static "; flag2=1;}
	| AUTO		{ $$ = createASTNode($1); currentDataType="auto "; flag2=1;}
	| REGISTER	{ $$ = createASTNode($1); currentDataType="register "; flag2=1;}
	;

type_specifier
	: VOID			{$$ = createASTNode($1); currentDataType="void";}	
	| CHAR			{$$ = createASTNode($1); if(flag2){currentDataType+=" char";flag2=0;}else{currentDataType="char";}; }	
	| SHORT			{$$ = createASTNode($1); if(flag2){currentDataType+=" short";flag2=0;}else{currentDataType="short";}; }	
	| INT			{$$ = createASTNode($1); if(flag2){currentDataType+=" int";flag2=0;}else{currentDataType="int";} }
	| LONG			{$$ = createASTNode($1); if(flag2){currentDataType+=" long";flag2=0;}else{currentDataType="long";}; }
	| FLOAT			{$$ = createASTNode($1); if(flag2){currentDataType+=" float";flag2=0;}else{currentDataType="float";}; }
	| DOUBLE		{$$ = createASTNode($1); if(flag2){currentDataType+=" double";flag2=0;}else{currentDataType="double";}; }
	| SIGNED		{$$ = createASTNode($1); currentDataType="signed"; flag2=1; }
	| UNSIGNED		{$$ = createASTNode($1); currentDataType="unsigned"; flag2=1; }
	| FILE_MAN          { $$ = createASTNode($1); currentDataType="file";  }
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
		currentDataType="Class ";
		std::string check=std::string($2);
		currentDataType+=check;
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
	| struct_or_union IDENTIFIER '{' '}'	{
		yyerror("syntax error, struct must be non-empty");
		$$ = createASTNode("Invalid Struct", nullptr);
	}
	;

struct_or_union
	: STRUCT	{$$ = $1; currentDataType="struct";}
	| UNION		{$$ = $1; currentDataType="union";}
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
		currentDataType="Enum ";
		std::string check=std::string($2);
		currentDataType+=check;
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
	: CONST		{ $$ = createASTNode($1); currentDataType="const "; flag2=1;}
	| VOLATILE	{ $$ = createASTNode($1); currentDataType="volatile "; flag2=1;}
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
		}
	}
	| CONSTANT IDENTIFIER {
		yyerror("syntax error, invalid identifier");
		$$ = createASTNode("Invalid Identifier");
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
		updateLastSymbolEntry();
		$$ = createASTNode("direct_declarator", &v);
	}
	| direct_declarator '(' parameter_type_list ')'{
		std::vector<Data> v, v2;
		insertAttr(v2, $3, "", 1);
		Node* node = createASTNode("( )", &v2);
		insertAttr(v, $1, "", 1);
		insertAttr(v, node, "", 1);
		$$ = createASTNode("direct_declarator", &v);
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
		noArgs++;
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		std::vector<Data> v;
		insertAttr(v, $1, "", 1);
		insertAttr(v, $3, "", 1);
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
	| IF '(' ')' statement {
        yyerror("syntax error, missing condition in 'if' statement.");
        $$ = createASTNode("error-node");
    }
	| SWITCH '(' ')' statement {
        yyerror("syntax error, missing condition in 'switch' statement.");
        $$ = createASTNode("error-node");
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
	| WHILE '[' expression ']' statement {
        yyerror("syntax error, incorrect parentheses in while-loop.");
		$$ = createASTNode("Invalid While-loop", nullptr);
    }
    | UNTIL '[' expression ']' statement {
        yyerror("syntax error, incorrect parentheses in until-loop.");
		$$ = createASTNode("Invalid Until-loop", nullptr);
    }
    | FOR '(' expression ',' expression ',' expression ')' statement {
        yyerror("syntax error, comma used instead of semicolons.");
        $$ = createASTNode("Invalid for-loop", nullptr);
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
	| error ';' {
		$$ = new Node; yyerrok;
	}
	| error ','{
		$$ = new Node; yyerrok;
	}
	| error{
		$$ = new Node; yyerrok;
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
	onlyLexin = false;
    beginAST(inputFile);
    yyparse();
    endAST();
}

int yyerror(const char* s) 
{
	yyclearin;
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
