
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "AST.hpp"
#include "parser.hpp"
#include "data_structures.hpp"
#include "typecheck.h"

extern std::string outputDir;
extern bool has_error;
void print_error(const std::string &message);

void count();
void reportError();
int check_type();
void set_int_type(int);
void set_real_type();
int column = 0;
int line = 1;
bool err_dur_parsing = false;
std::string current_token_type;
%}

%option noyywrap
%%

"/*"([^*]|(\*+[^*/]))*\*+\/	{ count();}

"//"[^\n]*		{ count(); }

"auto"          { count(); yylval.str = strdup(yytext); current_token_type="AUTO"; return(AUTO); }
"break"         { count(); yylval.str = strdup(yytext); current_token_type="BREAK"; return(BREAK); }
"case"          { count(); yylval.str = strdup(yytext); current_token_type="CASE"; return(CASE); }
"char"          { count(); yylval.str = strdup(yytext); current_token_type="CHAR"; return(CHAR); }
"const"         { count(); yylval.str = strdup(yytext); current_token_type="CONST"; return(CONST); }
"continue"      { count(); yylval.str = strdup(yytext); current_token_type="CONTINUE"; return(CONTINUE); }
"default"       { count(); yylval.str = strdup(yytext); current_token_type="DEFAULT"; return(DEFAULT); }
"do"            { count(); yylval.str = strdup(yytext); current_token_type="DO"; return(DO); }
"double"        { count(); yylval.str = strdup(yytext); current_token_type="DOUBLE"; return(DOUBLE); }
"else"          { count(); yylval.str = strdup(yytext); current_token_type="ELSE"; return(ELSE); }
"enum"          { count(); yylval.str = strdup(yytext); current_token_type="ENUM"; return(ENUM); }
"extern"        { count(); yylval.str = strdup(yytext); current_token_type="EXTERN"; return(EXTERN); }
"float"         { count(); yylval.str = strdup(yytext); current_token_type="FLOAT"; return(FLOAT); }
"for"           { count(); yylval.str = strdup(yytext); current_token_type="FOR"; return(FOR); }
"goto"          { count(); yylval.str = strdup(yytext); current_token_type="GOTO"; return(GOTO); }
"if"            { count(); yylval.str = strdup(yytext); current_token_type="IF"; return(IF); }
"int"           { count(); yylval.str = strdup(yytext); current_token_type="INT"; return(INT); }
"long"          { count(); yylval.str = strdup(yytext); current_token_type="LONG"; return(LONG); }
"register"      { count(); yylval.str = strdup(yytext); current_token_type="REGISTER"; return(REGISTER); }
"return"        { count(); yylval.str = strdup(yytext); current_token_type="RETURN"; return(RETURN); }
"short"         { count(); yylval.str = strdup(yytext); current_token_type="SHORT"; return(SHORT); }
"signed"        { count(); yylval.str = strdup(yytext); current_token_type="SIGNED"; return(SIGNED); }
"sizeof"        { count(); yylval.str = strdup(yytext); current_token_type="SIZEOF"; return(SIZEOF); }
"static"        { count(); yylval.str = strdup(yytext); current_token_type="STATIC"; return(STATIC); }
"struct"        { count(); yylval.str = strdup(yytext); current_token_type="STRUCT"; return(STRUCT); }
"switch"        { count(); yylval.str = strdup(yytext); current_token_type="SWITCH"; return(SWITCH); }
"typedef"       { count(); yylval.str = strdup(yytext); current_token_type="TYPEDEF"; return(TYPEDEF); }
"union"         { count(); yylval.str = strdup(yytext); current_token_type="UNION"; return(UNION); }
"unsigned"      { count(); yylval.str = strdup(yytext); current_token_type="UNSIGNED"; return(UNSIGNED); }
"until"         { count(); yylval.str = strdup(yytext); current_token_type="UNTIL"; return(UNTIL); }
"void"          { count(); yylval.str = strdup(yytext); current_token_type="VOID"; return(VOID); }
"volatile"      { count(); yylval.str = strdup(yytext); current_token_type="VOLATILE"; return(VOLATILE); }
"while"         { count(); yylval.str = strdup(yytext); current_token_type="WHILE"; return(WHILE); }
"class"          { count(); yylval.str = strdup(yytext); current_token_type="CLASS"; return(CLASS); }
"public"         { count(); yylval.str = strdup(yytext); current_token_type="PUBLIC"; return(PUBLIC); }
"private"        { count(); yylval.str = strdup(yytext); current_token_type="PRIVATE"; return(PRIVATE); }
"protected"      { count(); yylval.str = strdup(yytext); current_token_type="PROTECTED"; return(PROTECTED); }
"FILE"          { count(); yylval.str = strdup(yytext); current_token_type="FILE_MAN"; return(FILE_MAN); }
"va_list"       { count(); yylval.str = strdup(yytext); current_token_type="VA_LIST"; return(VA_LIST); }


{L}({L}|{D})*   { count(); yylval.str = strdup(yytext); current_token_type="IDENTIFIER"; return(check_type()); }

0[xX]{H}+{IS}?  { count();  current_token_type="CONSTANT";set_int_type(3);  if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Integer Constant"); return(CONSTANT); }
0{D}+{IS}?      { count();  current_token_type="CONSTANT"; set_int_type(2); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Integer Constant"); return(CONSTANT); }
{D}+{IS}?       { count(); current_token_type="CONSTANT"; set_int_type(1); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Integer Constant"); return(CONSTANT); }
L?'(\\.|[^\\'])+'  { count();  current_token_type="CONSTANT"; set_int_type(0); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Character Constant"); return(CONSTANT); }

{D}+{E}{FS}?    { count();  current_token_type="CONSTANT"; set_real_type(); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Float Constant"); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count();  current_token_type="CONSTANT"; set_real_type(); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Float Constant"); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?  { count(); current_token_type="CONSTANT"; set_real_type(); if(!onlyLexin) addToConstantTable(std::string(strdup(yytext)),"Float Constant"); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"  { count(); yylval.str = strdup(yytext); current_token_type="STRING_LITERAL"; return(STRING_LITERAL); }

"..."          { count(); yylval.str = strdup(yytext); current_token_type="ELLIPSIS"; return(ELLIPSIS); }
">>="          { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_ASSIGN"; return(RIGHT_ASSIGN); }
"<<="          { count(); yylval.str = strdup(yytext); current_token_type="LEFT_ASSIGN"; return(LEFT_ASSIGN); }
"+="           { count(); yylval.str = strdup(yytext); current_token_type="ADD_ASSIGN"; return(ADD_ASSIGN); }
"-="           { count(); yylval.str = strdup(yytext); current_token_type="SUB_ASSIGN"; return(SUB_ASSIGN); }
"*="           { count(); yylval.str = strdup(yytext); current_token_type="MUL_ASSIGN"; return(MUL_ASSIGN); }
"/="           { count(); yylval.str = strdup(yytext); current_token_type="DIV_ASSIGN"; return(DIV_ASSIGN); }
"%="           { count(); yylval.str = strdup(yytext); current_token_type="MOD_ASSIGN"; return(MOD_ASSIGN); }
"&="           { count(); yylval.str = strdup(yytext); current_token_type="AND_ASSIGN"; return(AND_ASSIGN); }
"^="           { count(); yylval.str = strdup(yytext); current_token_type="XOR_ASSIGN"; return(XOR_ASSIGN); }
"|="           { count(); yylval.str = strdup(yytext); current_token_type="OR_ASSIGN"; return(OR_ASSIGN); }
">>"           { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_OP"; return(RIGHT_OP); }
"<<"           { count(); yylval.str = strdup(yytext); current_token_type="LEFT_OP"; return(LEFT_OP); }
"++"           { count(); yylval.str = strdup(yytext); current_token_type="INC_OP"; return(INC_OP); }
"--"           { count(); yylval.str = strdup(yytext); current_token_type="DEC_OP"; return(DEC_OP); }
"->"           { count(); yylval.str = strdup(yytext); current_token_type="PTR_OP"; return(PTR_OP); }
"<-"           { count(); yylval.str = strdup(yytext); current_token_type = "INHERITANCE_OP"; return(INHERITANCE_OP); }
"&&"           { count(); yylval.str = strdup(yytext); current_token_type="AND_OP"; return(AND_OP); }
"||"           { count(); yylval.str = strdup(yytext); current_token_type="OR_OP"; return(OR_OP); }
"<="           { count(); yylval.str = strdup(yytext); current_token_type="LE_OP"; return(LE_OP); }
">="           { count(); yylval.str = strdup(yytext); current_token_type="GE_OP"; return(GE_OP); }
"=="           { count(); yylval.str = strdup(yytext); current_token_type="EQ_OP"; return(EQ_OP); }
"!="           { count(); yylval.str = strdup(yytext); current_token_type="NE_OP"; return(NE_OP); }
";"            { count(); yylval.str = strdup(yytext); current_token_type=";"; return(';'); }
("{"|"<%")     { count(); yylval.str = strdup(yytext); current_token_type="{"; return('{'); }
("}"|"%>")     { count(); yylval.str = strdup(yytext); current_token_type="}"; return('}'); }
","            { count(); yylval.str = strdup(yytext); current_token_type=","; return(','); }
":"            { count(); yylval.str = strdup(yytext); current_token_type=":"; return(':'); }
"="            { count(); yylval.str = strdup(yytext); current_token_type="="; return('='); }
"("            { count(); yylval.str = strdup(yytext); current_token_type="("; return('('); }
")"            { count(); yylval.str = strdup(yytext); current_token_type=")"; return(')'); }
("["|"<:")     { count(); yylval.str = strdup(yytext); current_token_type="["; return('['); }
("]"|":>")     { count(); yylval.str = strdup(yytext); current_token_type="]"; return(']'); }
"."            { count(); yylval.str = strdup(yytext); current_token_type="."; return('.'); }
"&"            { count(); yylval.str = strdup(yytext); current_token_type="&"; return('&'); }
"!"            { count(); yylval.str = strdup(yytext); current_token_type="!"; return('!'); }
"~"            { count(); yylval.str = strdup(yytext); current_token_type="~"; return('~'); }
"-"            { count(); yylval.str = strdup(yytext); current_token_type="-"; return('-'); }
"+"            { count(); yylval.str = strdup(yytext); current_token_type="+"; return('+'); }
"*"            { count(); yylval.str = strdup(yytext); current_token_type="*"; return('*'); }
"/"            { count(); yylval.str = strdup(yytext); current_token_type="/"; return('/'); }
"%"            { count(); yylval.str = strdup(yytext); current_token_type="%"; return('%'); }
"<"            { count(); yylval.str = strdup(yytext); current_token_type="<"; return('<'); }
">"            { count(); yylval.str = strdup(yytext); current_token_type=">"; return('>'); }
"^"            { count(); yylval.str = strdup(yytext); current_token_type="^"; return('^'); }
"|"            { count(); yylval.str = strdup(yytext); current_token_type="|"; return('|'); }
"?"            { count(); yylval.str = strdup(yytext); current_token_type="?"; return('?'); }

[ \t\v\n\f]	   { count(); }

.			   { count(); reportError();}

%%

void set_int_type(int t){
	// 3-> Hexa, 2-> Octal, 1-> decimal, 0-> Char
	yylval.num = new constants ;
	yylval.num->str = strdup(yytext);
	yylval.num->type = string("long long");
	switch(t){
		case 0 : {
			sscanf(yytext, "%lld", &(yylval.num->intVal));
			break;
		}
		case 1 : {
			sscanf(yytext, "%lld", &(yylval.num->intVal));
			break;
		}
		case 2 : {
			sscanf(yytext, "%llo", &(yylval.num->intVal));
			break;
		}
		case 3 : {
			sscanf(yytext, "%llx", &(yylval.num->intVal));
			break;
		}
	}
}

void set_real_type(){
	yylval.num = new constants ;
	yylval.num->str = strdup(yytext);
	yylval.num->type = string("long double");
	sscanf(yytext, "%Lf", &(yylval.num->realVal));
}
void reportError() 
{
    has_error = true;
    if(!err_dur_parsing)
	    std::cerr<<"\033[1;31merror: \033[0munknown token "<<yytext<<" at line "<<line<<" column "<<column - yyleng<<"\n";
}

void count()
{
	for (int i = 0; yytext[i] != '\0'; i++)
	{
		if (yytext[i] == '\n')
			column = 1, line++;
		else if (yytext[i] == '\t') // tab size : 4
			column += 4 - (column % 4);
		else
			column++;
	}
}


int check_type()
{
    std::string str = searchTypedefTable(strdup(yytext));
    std::unordered_map<std::string, int> primitiveTypes = {
        {"int", INT},
        {"char", CHAR},
        {"float", FLOAT},
        {"double", DOUBLE},
        {"void", VOID},
        {"short", SHORT},
        {"long", LONG},
        {"signed", SIGNED},
        {"unsigned", UNSIGNED}
    };

    if (primitiveTypes.find(str) != primitiveTypes.end()) {
        return primitiveTypes[str];
    }
    return IDENTIFIER;
}

void performLexing(const std::string &inputFile, bool lexPrint)
{
    onlyLexin = true;
    std::vector<TOKEN> tokens;
    int token_id;
    while ((token_id = yylex()) != 0)
    {
        TOKEN token;
        token.token_type = current_token_type;
        token.lexeme = std::string(yytext);
        token.line = line;
        token.column = column - yyleng;
        tokens.push_back(token);
    }

    if(lexPrint)
    {
        std::ofstream out(outputDir + inputFile + "_lexfile.txt");
        if (!out)
        {
            print_error("cannot open " + outputDir + inputFile + "_lexfile.txt");
            return;
        }
        out << "Lexical Analysis for " << inputFile << "\n\n";
        out << std::left
            << std::setw(20) << "Token"
            << std::setw(40) << "Lexeme"
            << std::setw(10) << "Line"
            << std::setw(10) << "Column" << "\n"
            << std::string(80, '-') << "\n";

        for (auto &token : tokens)
        {
            out << std::left
                << std::setw(20) << token.token_type
                << std::setw(40) << token.lexeme
                << std::setw(10) << token.line
                << std::setw(10) << token.column << "\n";
        }
        out.close();
    }
    line = 1;
    column = 0;
    err_dur_parsing = true;
    rewind(yyin);
    yyrestart(yyin);
}
