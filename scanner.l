%{
#include <iostream>
#include <string.h>
#include "tokens.hpp"

extern FILE *yyin;

int line_num = 1;
int col_num = 1;

class yyFlexLexer;

void update_line() {
		line_num++;
		col_num = 1;
}

void update_col(yyFlexLexer* scanner) {
		col_num += scanner->YYLeng();
}

%}

%option c++ header-file="scanner.hpp"

letra	[a-zA-Z]
digito	[0-9]
id		{letra}({letra}|{digito}|_)*
numero	{digito}+

%%
"program"	{update_col(this); return TOKEN_PROGRAM; }
"var"		{update_col(this); return TOKEN_VAR; }
"integer"	{update_col(this); return TOKEN_INTEGER; }
"boolean"	{update_col(this); return TOKEN_BOOLEAN; }
"procedure" {update_col(this); return TOKEN_PROCEDURE; }
"function"	{update_col(this); return TOKEN_FUNCTION; }
"begin"		{update_col(this); return TOKEN_BEGIN; }
"end"		{update_col(this); return TOKEN_END; }
"if"		{update_col(this); return TOKEN_IF; }
"then"		{update_col(this); return TOKEN_THEN; }
"else"		{update_col(this); return TOKEN_ELSE; }
"while"		{update_col(this); return TOKEN_WHILE; }
"do"		{update_col(this); return TOKEN_DO; }
"read"		{update_col(this); return TOKEN_READ; }
"write"		{update_col(this); return TOKEN_WRITE; }
"true"		{update_col(this); return TOKEN_TRUE; }
"false"		{update_col(this); return TOKEN_FALSE; }
"or"		{update_col(this); return TOKEN_OR; }
"not"		{update_col(this); return TOKEN_NOT; }
"div"		{update_col(this); return TOKEN_DIV; }

{id}		{update_col(this); return TOKEN_ID; }
{numero}	{update_col(this); return TOKEN_NUM; }

":="  		{update_col(this); return TOKEN_ASSIGN; }
"<>"		{update_col(this); return TOKEN_NEQ; }
"<"			{update_col(this); return TOKEN_LT; }
"<="		{update_col(this); return TOKEN_LTE; }
">"			{update_col(this); return TOKEN_GT; }
">="		{update_col(this); return TOKEN_GTE; }	

"+"|"-"|"*"|"="|"<"|">"|"("|")"|";"|","|":"|"." { update_col(this); return yytext[0]; }

[ \t]+		{update_col(this); }
\n			{update_line(); }

<<EOF>> 	{return 0;}

.	{
		std::cerr << "Erro na linha " << line_num << ", coluna " << col_num << ": caractere inesperado " << YYText() <<"'\n";
		update_col(this);
}

%%

int yyFlexLexer::yywrap() {
		return 1;
}
