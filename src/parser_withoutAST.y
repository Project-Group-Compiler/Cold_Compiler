%{
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstring>

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
extern std::string current_token_type;

int yyerror(const char*);
int yylex();
int onlyLexer = 0;

extern int yyleng;
extern int yylex();
extern int yyrestart(FILE*);
extern FILE* yyin;
#define YYERROR_VERBOSE
%}

%union{
	char* str;
	int number;
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

%type<str> primary_expression postfix_expression argument_expression_list unary_expression unary_operator cast_expression multiplicative_expression additive_expression shift_expression relational_expression equality_expression
%type<str> assignment_operator 
%type<str> and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression 
%type<str> assignment_expression expression constant_expression declaration declaration_specifiers init_declarator_list
%type<str> declarator direct_declarator pointer type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator direct_abstract_declarator initializer
%type<str> init_declarator type_specifier struct_or_union_specifier	struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator enum_specifier enumerator_list enumerator type_qualifier
%type<str> statement labeled_statement compound_statement declaration_list statement_list expression_statement selection_statement iteration_statement jump_statement translation_unit external_declaration function_definition initializer_list
%type<str> storage_class_specifier
%type<str> struct_or_union
%type<str> class_definition inheritance_specifier inheritance_specifier_list access_specifier class class_definition_head class_internal_definition_list class_internal_definition	class_member_list class_member

%left ';'
%expect 1 

%%

primary_expression
    : IDENTIFIER {
    	$$ = $1;
    }
	| CONSTANT 	{
		$$ = $1;
	}
	| STRING_LITERAL {
		$$ = $1;
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
		$$ = $1;
	}
	| postfix_expression '(' ')' {
		$$ = $1;
	}
	| postfix_expression '(' argument_expression_list ')' {
		$$ = $1;
	}
	| postfix_expression '.' IDENTIFIER {
		$$ = $1;
	}
	| postfix_expression PTR_OP IDENTIFIER {
		$$ = $1;
	}
	| postfix_expression INC_OP {
		$$ = $1;
	}
	| postfix_expression DEC_OP {
		$$ = $1;
	}
	;

argument_expression_list
	: assignment_expression {
		$$ = $1;
	}
	| argument_expression_list ',' assignment_expression {
		$$ = $1;
	}
	;

unary_expression
	: postfix_expression {
		$$ = $1;
	}
	| INC_OP unary_expression {
		$$ = $2;
	}
	| DEC_OP unary_expression {
		$$ = $2;
	}
	| unary_operator cast_expression {
		$$ = $2;
	}
	| SIZEOF unary_expression {
		$$ = $2;
	}
	| SIZEOF '(' type_name ')' {
		$$ = $3;
	}
	;

unary_operator
	: '&' {
		$$ = "&";
	}
	| '*' {
		$$ = "*";
	}
	| '+' {
		$$ = "+";
	}
	| '-' {
		$$ = "-";
	}
	| '~' {
		$$ = "~";
	}
	| '!' {
		$$ = "!";
	}
	;

cast_expression
	: unary_expression {
		$$ = $1;
	}
	| '(' type_name ')' cast_expression {
		$$ = $4;
	}
	;

multiplicative_expression
	: cast_expression {
		$$ = $1;
	}
	| multiplicative_expression '*' cast_expression {
		$$ = $1;
	}
	| multiplicative_expression '/' cast_expression {
		$$ = $1;
	}
	| multiplicative_expression '%' cast_expression {
		$$ = $1;
	}
	;

additive_expression
	: multiplicative_expression {
		$$ = $1;
	}
	| additive_expression '+' multiplicative_expression {
		$$ = $1;
	}
	| additive_expression '-' multiplicative_expression {
		$$ = $1;
	}
	;

shift_expression
	: additive_expression {
		$$ = $1;
	}
	| shift_expression LEFT_OP additive_expression {
		$$ = $1;
	}
	| shift_expression RIGHT_OP additive_expression {
		$$ = $1;
	}
	; 

relational_expression
	: inclusive_or_expression {
		 $$ = $1;
	}
	| relational_expression '<' inclusive_or_expression {
		$$ = $1;
	}
	| relational_expression '>' inclusive_or_expression {
		$$ = $1;
	}
	| relational_expression LE_OP inclusive_or_expression {
		$$ = $1;
	}
	| relational_expression GE_OP inclusive_or_expression {
		$$ = $1;
	}
	;

equality_expression
	: relational_expression { 
		$$ = $1;
	}
	| equality_expression EQ_OP relational_expression {
		$$ = $1;
	}
	| equality_expression NE_OP relational_expression {
		$$ = $1;
	}
	;

and_expression
	: shift_expression {	
		$$ = $1; 
	}
	| and_expression '&' shift_expression{
		$$ = $1;
	}
	;

exclusive_or_expression
	: and_expression {
		$$ = $1;
	}
	| exclusive_or_expression '^' and_expression{
        $$ = $1;
    }
	;

inclusive_or_expression
	: exclusive_or_expression {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression{
		$$ = $1;
	}
	;

logical_and_expression
	: equality_expression {$$ = $1;}
	| logical_and_expression AND_OP equality_expression{
		$$ = $1;
	}
	;

logical_or_expression
	: logical_and_expression{$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression{
		$$ = $1;
	}
	;

conditional_expression
	: logical_or_expression {$$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression{
		$$ = $1;
	}
	;

assignment_expression
	: conditional_expression{$$ = $1;}
	| unary_expression assignment_operator assignment_expression{
		$$ = $1;
	}
	;

assignment_operator
	: '='	{
		$$ = "=";
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
		$$ = $1;
	}
	;

constant_expression
	: conditional_expression{$$ = $1;}
	;

declaration
	: declaration_specifiers ';'{ $$ = $1; }
	| declaration_specifiers init_declarator_list ';'{
		$$ = $1;
	}
	;

declaration_specifiers
	: storage_class_specifier { $$ = $1; }
	| storage_class_specifier declaration_specifiers{
		$$ = $1;
	}
	| type_specifier{ $$ = $1; }
	| type_specifier declaration_specifiers{
		$$ = $1;
	}
	| type_qualifier{ $$ = $1; }
	| type_qualifier declaration_specifiers{
		$$ = $1;
	}
	;

init_declarator_list
	: init_declarator{$$ = $1;}
	| init_declarator_list ',' init_declarator{
		$$ = $1;
	}
	;

init_declarator
	: declarator {$$ = $1;}
	| declarator '=' initializer{
		$$ = $1;
	}
	;

storage_class_specifier
	: TYPEDEF	{ $$ = $1;}
	| EXTERN	{ $$ = $1;}
	| STATIC	{ $$ = $1;}
	| AUTO		{ $$ = $1;}
	| REGISTER	{ $$ = $1;}
	;

type_specifier
	: VOID			{$$ = $1;}	
	| CHAR			{$$ = $1;}	
	| SHORT			{$$ = $1;}	
	| INT			{
		if (!$1) {
			yyerror("Parser error: NULL INT token received");
			YYABORT;
		}
		$$ = $1;
	}
	| LONG			{$$ = $1;}
	| FLOAT			{$$ = $1;}
	| DOUBLE		{$$ = $1;}
	| SIGNED		{$$ = $1;}
	| UNSIGNED		{$$ = $1;}
	| struct_or_union_specifier	{$$ = $1;}	
	| class_definition 			{$$ = $1;}
	| enum_specifier			{$$ = $1;}
	| TYPE_NAME		{$$ = $1;}	
	;

inheritance_specifier
	: access_specifier IDENTIFIER {
        $$ = $1;
    }
	;

inheritance_specifier_list
	: inheritance_specifier { $$ = $1; }
	| inheritance_specifier_list ',' inheritance_specifier{
        $$ = $1;
    }
	;

access_specifier 
	: PRIVATE { $$ = $1; }
	| PUBLIC { $$ = $1; }
	| PROTECTED { $$ = $1; }
	;

class
	: CLASS { $$ = $1; }
	;

class_definition_head 
	: class { $$ = $1;}
	| class INHERITANCE_OP inheritance_specifier_list{
        $$ = $1;
    }
	| class IDENTIFIER {
        $$ = $1;
    }
	| class IDENTIFIER  INHERITANCE_OP inheritance_specifier_list{
        $$ = $1;
    }
	;

class_definition 
	: class_definition_head '{' class_internal_definition_list '}'{
        $$ = $1;
    }
	| class_definition_head { $$ = $1; }
	;

class_internal_definition_list
	: class_internal_definition { $$ = $1; }
	| class_internal_definition_list class_internal_definition {
        $$ = $1;
    }
	; 

class_internal_definition	
	: access_specifier '{' class_member_list '}' ';'{
        $$ = $1;
    }
	;

class_member_list
	: class_member{ $$ = $1; }
	| class_member_list class_member{
        $$ = $1;
    }
	;

class_member
	: function_definition { $$ = $1; }
	| declaration { $$ = $1; }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{
		$$ = $1;
	}
	| struct_or_union '{' struct_declaration_list '}'		{
		$$ = $1;
	}
	| struct_or_union IDENTIFIER 	{
		$$ = $1;
	}
	;

struct_or_union
	: STRUCT	{$$ = $1;}
	| UNION		{$$ = $1;}
	;

struct_declaration_list
	: struct_declaration	{ $$ = $1 ;}
	| struct_declaration_list struct_declaration 	{
		$$ = $1;
	}
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' 	{
		$$ = $1;
	}
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{
		$$ = $1;
	}
	| type_specifier	{ $$ = $1; }
	| type_qualifier specifier_qualifier_list 	{
		$$ = $1;
	}
	| type_qualifier	{ $$ = $1; }
	;

struct_declarator_list
	: struct_declarator { $$ = $1; }
	| struct_declarator_list ',' struct_declarator {
		$$ = $1;
	}
	;

struct_declarator
	: declarator	{ $$ = $1; }
	| ':' constant_expression	{ $$ = $2; }
	| declarator ':' constant_expression	{
		$$ = $1;
	}
	;

enum_specifier
	: ENUM '{' enumerator_list '}'		{
		$$ = $1;
	}
	| ENUM IDENTIFIER '{' enumerator_list '}'	{
		$$ = $1;
	}
	| ENUM IDENTIFIER {
		$$ = $1;
	}
	;

enumerator_list
	: enumerator 	{ $$ = $1; }
	| enumerator_list ',' enumerator 	{
		$$ = $1;
	}
	;

enumerator
	: IDENTIFIER	{ $$ = $1; }
	| IDENTIFIER '=' constant_expression 	{
		$$ = $1;
	}
	;

type_qualifier
	: CONST		{ $$ = $1; }
	| VOLATILE	{ $$ = $1; }
	;


declarator
	: pointer direct_declarator{
		$$ = $1;
	}
	| direct_declarator {
		$$ = $1 ;
	}
	;


direct_declarator
	: IDENTIFIER {
		$$ = $1;
	}
	| '(' declarator ')'  {
		$$ = $2 ;
	}
	| direct_declarator '[' constant_expression ']'{
		$$ = $1;
	}
	| direct_declarator '[' ']'{
		$$ = $1;
	}
	| direct_declarator '(' parameter_type_list ')'{
		$$ = $1;
	}
	| direct_declarator '(' identifier_list ')'{
		$$ = $1;
	}
	| direct_declarator '(' ')'{
		$$ = $1;
	}
	;

pointer
	: '*' {
		$$ = "*(Pointer)";
	}
	| '*' type_qualifier_list{
		$$ = $1;
	}
	| '*' pointer{
		$$ = $1;
	}
	| '*' type_qualifier_list pointer{
		$$ = $1;
	}
	;

type_qualifier_list
	: type_qualifier {
		$$ = $1 ;
	}
	| type_qualifier_list type_qualifier{
		$$ = $1;
	}
	;


parameter_type_list
	: parameter_list {
		$$ = $1 ;
	}
	| parameter_list ',' ELLIPSIS{
		$$ = $1;
	}
	;

parameter_list
	: parameter_declaration{
		$$ = $1;
	}
	| parameter_list ',' parameter_declaration{
		$$ = $1;
	}
	;

parameter_declaration
	: declaration_specifiers declarator{
		$$ = $1;
	}
	| declaration_specifiers abstract_declarator{
		$$ = $1;
	}
	| declaration_specifiers {
		$$ = $1;
	}
	;

identifier_list
	: IDENTIFIER {
		$$ = $1;
	}
	| identifier_list ',' IDENTIFIER{
		$$ = $1;
	}
	;

type_name
	: specifier_qualifier_list{
		$$ = $1;
	}
	| specifier_qualifier_list abstract_declarator{
		$$ = $1;
	}
	;

abstract_declarator
	: pointer {
		$$ = $1;
	}
	| direct_abstract_declarator{
		$$ = $1;
	}
	| pointer direct_abstract_declarator{
		$$ = $1;
	}
	;

direct_abstract_declarator
	: '(' abstract_declarator ')' {
		$$ = $2;
	}
	| '[' ']'{
		$$ = "[ ]" ;
	}
	| '[' constant_expression ']' {
		$$ = $2;
	}
	| direct_abstract_declarator '[' ']' {
		$$ = $1;
	}
	| direct_abstract_declarator '[' constant_expression ']'{
		$$ = $1;
	}
	| '(' ')'{
		$$ = "( )" ;
	}
	| '(' parameter_type_list ')'{
		$$ = $2 ;
	}
	| direct_abstract_declarator '(' ')'{
		$$ = $1;
	}
	| direct_abstract_declarator '(' parameter_type_list ')'{
		$$ = $1;
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
		$$ = $1;
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
		$$ = $1;
	}
	| CASE constant_expression ':' statement	{
		$$ = $2;
	}
	| DEFAULT ':' statement	{
		$$ = $3;
	}
	;

compound_statement
	: '{' '}'	{$$ = "{ }";}
	| '{' statement_list '}'	{$$ = $2;}
	| '{' declaration_list '}'	{$$ = $2;}
	| '{' declaration_list statement_list '}'	{
		$$ = $2;
	}
	;

declaration_list
	: declaration	{$$ = $1;}
	| declaration_list declaration	{
		$$ = $1;
	}
	;

statement_list
	: statement	{$$ = $1;}
	| statement_list statement	{
		$$ = $1;
	}
	;

expression_statement
	: ';'	{$$ = ";";}
	| expression ';'	{$$ = $1;}
	;

selection_statement
	: IF '(' expression ')' statement	{
		$$ = $3;
	}
	| IF '(' expression ')' statement ELSE statement	{
		$$ = $3;
	}
	| SWITCH '(' expression ')' statement	{
		$$ = $3;
	}
	;

iteration_statement
	: WHILE '(' expression ')' statement	{
		$$ = $3;
	}
	| DO statement WHILE '(' expression ')' ';'	{
		$$ = $5;
	}
	| FOR '(' expression_statement expression_statement ')' statement	{
		$$ = $3;
	}
	| FOR '(' expression_statement expression_statement expression ')' statement	{
		$$ = $3;
	}
    | UNTIL '(' expression ')' statement { /*** Added UNTIL grammar ***/
		$$ = $3;
	}
	;

jump_statement
	: GOTO IDENTIFIER ';'	{
        $$ = $1;
	}
	| CONTINUE ';'	{$$ = $1;}
	| BREAK ';'		{$$ = $1;}
	| RETURN ';'	{$$ = $1;}
	| RETURN expression ';'	{
		$$ = $1;
	}
	;

translation_unit
	: external_declaration	{
		$$ = $1;
	}
	| translation_unit external_declaration	{
		$$ = $1;
	}
	;

external_declaration
	: function_definition	{ $$ = $1;}
	| declaration	{ $$ = $1;}
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement	{
		$$ = $1;
	}
	| declaration_specifiers declarator compound_statement	{
		$$ = $1;
	}
	| declarator declaration_list compound_statement	{
                $$ = $1;
	}
	| declarator compound_statement	{
		$$ = $1;
	}
	;

%%

std::string lexerOutputFile;
std::string currentFilename; 

void print_error(const std::string& message) {	
    std::cerr << "\033[1;31mError: \033[0m" << message << "\n";
}

void print_options() {
	//Later
}

void performLexicalAnalysis(const std::string& filename) {
    std::ofstream out(lexerOutputFile);
    if (!out) {
        print_error("Cannot open output file " + lexerOutputFile);
        return;
    }

    std::unordered_map<std::string, TOKEN> symbol_table;
    std::vector<TOKEN> tokens;
    
    int token_id;
    while ((token_id = yylex()) != 0) {
        TOKEN token;
        token.token_type = current_token_type;
        token.lexeme = std::string(yytext);
        token.line = line;
        token.column = column - yyleng;

        if (symbol_table.find(token.lexeme) == symbol_table.end()) {
            symbol_table[token.lexeme] = token;
        }
        tokens.push_back(token);
    }

    out << "Lexical Analysis for " << filename << "\n\n";
    out << std::left 
        << std::setw(20) << "Token" 
        << std::setw(40) << "Lexeme" 
        << std::setw(10) << "Line" 
        << std::setw(10) << "Column" << "\n"
        << std::string(80, '-') << "\n";

    for (auto& token : tokens) {
        out << std::left 
            << std::setw(20) << token.token_type
            << std::setw(40) << token.lexeme
            << std::setw(10) << token.line
            << std::setw(10) << token.column << "\n";
    }

    out.close();
}

void performParsing() {
    dotfile = fopen("src/AST.dot", "w");
    if (!dotfile) {
        print_error("Cannot open AST.dot for writing.");
        return;
    }

    beginAST();
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

    return 0;
}

// Error handling function
int yyerror(const char* s) {
    std::cerr << "\033[1;31mError:\033[0m " << s << " at line " << line 
              << ", column " << column + 1 - (yytext ? strlen(yytext) : 0) 
              << std::endl;

    if (currentFilename.empty()) {
        print_error("Filename not set.");
        return -1;
    }

    std::ifstream file(currentFilename);  
    if (!file) {
        print_error("Cannot open source file: " + currentFilename);
        return -1;
    }

    std::string curr_line;
    int count = 1;
    
    while (std::getline(file, curr_line)) {
        if (count == line) {
            std::cerr << "\n" << line << " | " << curr_line << "\n";
            std::cerr << "    " << std::string(column - yyleng + 1, ' ') << "^\n";
            std::cerr << "\033[1;31mError: \033[0m" << s << "\n\n";
            break;
        }
        count++;
    }

    file.close();
    return -1;
}