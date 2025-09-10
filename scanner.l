%{
#include <iostream>
#include <string.h>
#include "tokens.hpp"

%option c++ header-file="scanner.hpp"

extern FILE *yyin;

int line_num = 1;
int col_num = 1;

void update_line() {
		line_num++;
		col_num = 1;
}

void update_col() {
		col_num += yyleng;
}

%}

letra	[a-zA-Z]
digito	[0-9]
id		{letra}({letra}|{digito}|_)
numero	{digito}+

%%
"program"	{update_col(); return TOKEN_PROGRAM; }
"var"		{update_col(); return TOKEN_VAR; }
"integer"	{update_col(); return TOKEN_INTEGER; }
"boolean"	{update_col(); return TOKEN_BOOLEAN; }
"procedure" {update_col(); return TOKEN_PROCUDERE; }
"function"	{update_col(); return TOKEN_FUNCTION; }
"begin"		{update_col(); return TOKEN_BEGIN; }
"end"		{update_col(); return TOKEN_END; }
"if"		{update_col(); return TOKEN_IF; }
"then"		{update_col(); return TOEKN_THEN; }
"else"		{update_col(); return TOKEN_ELSE; }
"while"		{update_col(); return TOKEN_WHILE; }
"do"		{update_col(); return TOKEN_DO; }
"read"		{rpdate_col(); return TOKEN_READ; }
"write"		{update_col(); return TOKEN_WRITE; }
"true"		{update_col(); return TOKEN_TRUE; }
"false"		{update_col(); return TOKEN_FALSE; }
"or"		{update_col(); return TOKEN_OR; }
"not"		{update_col(); return TOKEN_NOT; }
"div"		{update_col(); return TOKEN_DIV; }

{id}		{update_col(); return TOKEN_ID; }
{numero}	{update_col(); return TOKEN_NUM; }

":="  		{update_col(); return TOKEN_ASSING; }
"<>"		{update_col(); return TOKEN_NEQ; }
"<"			{update_col(); return TOKEN_LT; }
"<="		{update_col(); return TOKEN_LTE; }
">"			{update_col(); return TOKEN_GT; }
">="		{update_col(); return TOKEN_GTE; }	

"+"|"-"|"*"|"="|"<"|">"|"("|")"|";"|","|":"|"." { update_col(); return yytext[0]; }

[\t]+		{update_col(); }
\n			{update_line(); }

{
		std::cerr << "Erro na linha " << line_num << ", coluna " << col_num << ": caractere inesperado " << yytext <<"'\n";
}
%%
