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
#include <fstream>
#include "AST.hpp"
#include "parser.hpp"

struct TOKEN
{
    int line;
    int column;
    std::string token_type;
    std::string lexeme;
};

extern std::string outputDir;
extern bool has_error;
void print_error(const std::string &message);

void count();
void reportError();
int check_type();
int column = 0;
int line = 1;
bool err_dur_parsing = false;
std::string current_token_type;
std::string current_token_lexeme; // Added global variable to store current token lexeme
%}

%option noyywrap
%%

"/*"([^*]|(\*+[^*/]))*\*+\/	{ count();}

"//"[^\n]*		{ count(); }

"auto"          { count(); yylval.str = strdup(yytext); current_token_type="AUTO"; current_token_lexeme=yytext; return(AUTO); }
"break"         { count(); yylval.str = strdup(yytext); current_token_type="BREAK"; current_token_lexeme=yytext; return(BREAK); }
"case"          { count(); yylval.str = strdup(yytext); current_token_type="CASE"; current_token_lexeme=yytext; return(CASE); }
"char"          { count(); yylval.str = strdup(yytext); current_token_type="CHAR"; current_token_lexeme=yytext; return(CHAR); }
"const"         { count(); yylval.str = strdup(yytext); current_token_type="CONST"; current_token_lexeme=yytext; return(CONST); }
"continue"      { count(); yylval.str = strdup(yytext); current_token_type="CONTINUE"; current_token_lexeme=yytext; return(CONTINUE); }
"default"       { count(); yylval.str = strdup(yytext); current_token_type="DEFAULT"; current_token_lexeme=yytext; return(DEFAULT); }
"do"            { count(); yylval.str = strdup(yytext); current_token_type="DO"; current_token_lexeme=yytext; return(DO); }
"double"        { count(); yylval.str = strdup(yytext); current_token_type="DOUBLE"; current_token_lexeme=yytext; return(DOUBLE); }
"else"          { count(); yylval.str = strdup(yytext); current_token_type="ELSE"; current_token_lexeme=yytext; return(ELSE); }
"enum"          { count(); yylval.str = strdup(yytext); current_token_type="ENUM"; current_token_lexeme=yytext; return(ENUM); }
"extern"        { count(); yylval.str = strdup(yytext); current_token_type="EXTERN"; current_token_lexeme=yytext; return(EXTERN); }
"float"         { count(); yylval.str = strdup(yytext); current_token_type="FLOAT"; current_token_lexeme=yytext; return(FLOAT); }
"for"           { count(); yylval.str = strdup(yytext); current_token_type="FOR"; current_token_lexeme=yytext; return(FOR); }
"goto"          { count(); yylval.str = strdup(yytext); current_token_type="GOTO"; current_token_lexeme=yytext; return(GOTO); }
"if"            { count(); yylval.str = strdup(yytext); current_token_type="IF"; current_token_lexeme=yytext; return(IF); }
"int"           { count(); yylval.str = strdup(yytext); current_token_type="INT"; current_token_lexeme=yytext; return(INT); }
"long"          { count(); yylval.str = strdup(yytext); current_token_type="LONG"; current_token_lexeme=yytext; return(LONG); }
"register"      { count(); yylval.str = strdup(yytext); current_token_type="REGISTER"; current_token_lexeme=yytext; return(REGISTER); }
"return"        { count(); yylval.str = strdup(yytext); current_token_type="RETURN"; current_token_lexeme=yytext; return(RETURN); }
"short"         { count(); yylval.str = strdup(yytext); current_token_type="SHORT"; current_token_lexeme=yytext; return(SHORT); }
"signed"        { count(); yylval.str = strdup(yytext); current_token_type="SIGNED"; current_token_lexeme=yytext; return(SIGNED); }
"sizeof"        { count(); yylval.str = strdup(yytext); current_token_type="SIZEOF"; current_token_lexeme=yytext; return(SIZEOF); }
"static"        { count(); yylval.str = strdup(yytext); current_token_type="STATIC"; current_token_lexeme=yytext; return(STATIC); }
"struct"        { count(); yylval.str = strdup(yytext); current_token_type="STRUCT"; current_token_lexeme=yytext; return(STRUCT); }
"switch"        { count(); yylval.str = strdup(yytext); current_token_type="SWITCH"; current_token_lexeme=yytext; return(SWITCH); }
"typedef"       { count(); yylval.str = strdup(yytext); current_token_type="TYPEDEF"; current_token_lexeme=yytext; return(TYPEDEF); }
"union"         { count(); yylval.str = strdup(yytext); current_token_type="UNION"; current_token_lexeme=yytext; return(UNION); }
"unsigned"      { count(); yylval.str = strdup(yytext); current_token_type="UNSIGNED"; current_token_lexeme=yytext; return(UNSIGNED); }
"until"         { count(); yylval.str = strdup(yytext); current_token_type="UNTIL"; current_token_lexeme=yytext; return(UNTIL); }
"void"          { count(); yylval.str = strdup(yytext); current_token_type="VOID"; current_token_lexeme=yytext; return(VOID); }
"volatile"      { count(); yylval.str = strdup(yytext); current_token_type="VOLATILE"; current_token_lexeme=yytext; return(VOLATILE); }
"while"         { count(); yylval.str = strdup(yytext); current_token_type="WHILE"; current_token_lexeme=yytext; return(WHILE); }

{L}({L}|{D})*   { count(); yylval.str = strdup(yytext); current_token_type="IDENTIFIER"; current_token_lexeme=yytext; return(check_type()); }

0[xX]{H}+{IS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }
0{D}+{IS}?      { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }
{D}+{IS}?       { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }
L?'(\\.|[^\\'])+'  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }

{D}+{E}{FS}?    { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?  { count(); yylval.str = strdup(yytext); current_token_type="CONSTANT"; current_token_lexeme=yytext; return(CONSTANT); }

L?\"(\\.|[^\\"])*\"  { count(); yylval.str = strdup(yytext); current_token_type="STRING_LITERAL"; current_token_lexeme=yytext; return(STRING_LITERAL); }

"..."          { count(); yylval.str = strdup(yytext); current_token_type="ELLIPSIS"; current_token_lexeme=yytext; return(ELLIPSIS); }
">>="          { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_ASSIGN"; current_token_lexeme=yytext; return(RIGHT_ASSIGN); }
"<<="          { count(); yylval.str = strdup(yytext); current_token_type="LEFT_ASSIGN"; current_token_lexeme=yytext; return(LEFT_ASSIGN); }
"+="           { count(); yylval.str = strdup(yytext); current_token_type="ADD_ASSIGN"; current_token_lexeme=yytext; return(ADD_ASSIGN); }
"-="           { count(); yylval.str = strdup(yytext); current_token_type="SUB_ASSIGN"; current_token_lexeme=yytext; return(SUB_ASSIGN); }
"*="           { count(); yylval.str = strdup(yytext); current_token_type="MUL_ASSIGN"; current_token_lexeme=yytext; return(MUL_ASSIGN); }
"/="           { count(); yylval.str = strdup(yytext); current_token_type="DIV_ASSIGN"; current_token_lexeme=yytext; return(DIV_ASSIGN); }
"%="           { count(); yylval.str = strdup(yytext); current_token_type="MOD_ASSIGN"; current_token_lexeme=yytext; return(MOD_ASSIGN); }
"&="           { count(); yylval.str = strdup(yytext); current_token_type="AND_ASSIGN"; current_token_lexeme=yytext; return(AND_ASSIGN); }
"^="           { count(); yylval.str = strdup(yytext); current_token_type="XOR_ASSIGN"; current_token_lexeme=yytext; return(XOR_ASSIGN); }
"|="           { count(); yylval.str = strdup(yytext); current_token_type="OR_ASSIGN"; current_token_lexeme=yytext; return(OR_ASSIGN); }
">>"           { count(); yylval.str = strdup(yytext); current_token_type="RIGHT_OP"; current_token_lexeme=yytext; return(RIGHT_OP); }
"<<"           { count(); yylval.str = strdup(yytext); current_token_type="LEFT_OP"; current_token_lexeme=yytext; return(LEFT_OP); }
"++"           { count(); yylval.str = strdup(yytext); current_token_type="INC_OP"; current_token_lexeme=yytext; return(INC_OP); }
"--"           { count(); yylval.str = strdup(yytext); current_token_type="DEC_OP"; current_token_lexeme=yytext; return(DEC_OP); }
"->"           { count(); yylval.str = strdup(yytext); current_token_type="PTR_OP"; current_token_lexeme=yytext; return(PTR_OP); }
"&&"           { count(); yylval.str = strdup(yytext); current_token_type="AND_OP"; current_token_lexeme=yytext; return(AND_OP); }
"||"           { count(); yylval.str = strdup(yytext); current_token_type="OR_OP"; current_token_lexeme=yytext; return(OR_OP); }
"<="           { count(); yylval.str = strdup(yytext); current_token_type="LE_OP"; current_token_lexeme=yytext; return(LE_OP); }
">="           { count(); yylval.str = strdup(yytext); current_token_type="GE_OP"; current_token_lexeme=yytext; return(GE_OP); }
"=="           { count(); yylval.str = strdup(yytext); current_token_type="EQ_OP"; current_token_lexeme=yytext; return(EQ_OP); }
"!="           { count(); yylval.str = strdup(yytext); current_token_type="NE_OP"; current_token_lexeme=yytext; return(NE_OP); }
";"            { count(); yylval.str = strdup(yytext); current_token_type=";"; current_token_lexeme=yytext; return(';'); }
("{"|"<%")     { count(); yylval.str = strdup(yytext); current_token_type="{"; current_token_lexeme=yytext; return('{'); }
("}"|"%>")     { count(); yylval.str = strdup(yytext); current_token_type="}"; current_token_lexeme=yytext; return('}'); }
","            { count(); yylval.str = strdup(yytext); current_token_type=","; current_token_lexeme=yytext; return(','); }
":"            { count(); yylval.str = strdup(yytext); current_token_type=":"; current_token_lexeme=yytext; return(':'); }
"="            { count(); yylval.str = strdup(yytext); current_token_type="="; current_token_lexeme=yytext; return('='); }
"("            { count(); yylval.str = strdup(yytext); current_token_type="("; current_token_lexeme=yytext; return('('); }
")"            { count(); yylval.str = strdup(yytext); current_token_type=")"; current_token_lexeme=yytext; return(')'); }
("["|"<:")     { count(); yylval.str = strdup(yytext); current_token_type="["; current_token_lexeme=yytext; return('['); }
("]"|":>")     { count(); yylval.str = strdup(yytext); current_token_type="]"; current_token_lexeme=yytext; return(']'); }
"."            { count(); yylval.str = strdup(yytext); current_token_type="."; current_token_lexeme=yytext; return('.'); }
"&"            { count(); yylval.str = strdup(yytext); current_token_type="&"; current_token_lexeme=yytext; return('&'); }
"!"            { count(); yylval.str = strdup(yytext); current_token_type="!"; current_token_lexeme=yytext; return('!'); }
"~"            { count(); yylval.str = strdup(yytext); current_token_type="~"; current_token_lexeme=yytext; return('~'); }
"-"            { count(); yylval.str = strdup(yytext); current_token_type="-"; current_token_lexeme=yytext; return('-'); }
"+"            { count(); yylval.str = strdup(yytext); current_token_type="+"; current_token_lexeme=yytext; return('+'); }
"*"            { count(); yylval.str = strdup(yytext); current_token_type="*"; current_token_lexeme=yytext; return('*'); }
"/"            { count(); yylval.str = strdup(yytext); current_token_type="/"; current_token_lexeme=yytext; return('/'); }
"%"            { count(); yylval.str = strdup(yytext); current_token_type="%"; current_token_lexeme=yytext; return('%'); }
"<"            { count(); yylval.str = strdup(yytext); current_token_type="<"; current_token_lexeme=yytext; return('<'); }
">"            { count(); yylval.str = strdup(yytext); current_token_type=">"; current_token_lexeme=yytext; return('>'); }
"^"            { count(); yylval.str = strdup(yytext); current_token_type="^"; current_token_lexeme=yytext; return('^'); }
"|"            { count(); yylval.str = strdup(yytext); current_token_type="|"; current_token_lexeme=yytext; return('|'); }
"?"            { count(); yylval.str = strdup(yytext); current_token_type="?"; current_token_lexeme=yytext; return('?'); }

[ \t\v\n\f]	   { count(); }

.			   { count(); reportError();}

%%

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
	return(IDENTIFIER);
}

void performLexing(const std::string &inputFile, bool lexPrint)
{
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
