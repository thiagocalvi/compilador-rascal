%{
#include <iostream>
#include <string.h>
#include "wrapper.h"
#include "parser.tab.hpp"

%}

%option noyywrap

letra	[a-zA-Z]
digito	[0-9]
id		{letra}({letra}|{digito}|_)*
numero	{digito}+

%%
"program"	{update_col(yyleng); return TOKEN_PROGRAM; }
"var"		{update_col(yyleng); return TOKEN_VAR; }
"integer"	{update_col(yyleng); return TOKEN_INTEGER; }
"boolean"	{update_col(yyleng); return TOKEN_BOOLEAN; }
"procedure" {update_col(yyleng); return TOKEN_PROCEDURE; }
"function"	{update_col(yyleng); return TOKEN_FUNCTION; }
"begin"		{update_col(yyleng); return TOKEN_BEGIN; }
"end"		{update_col(yyleng); return TOKEN_END; }
"if"		{update_col(yyleng); return TOKEN_IF; }
"then"		{update_col(yyleng); return TOKEN_THEN; }
"else"		{update_col(yyleng); return TOKEN_ELSE; }
"while"		{update_col(yyleng); return TOKEN_WHILE; }
"do"		{update_col(yyleng); return TOKEN_DO; }
"read"		{update_col(yyleng); return TOKEN_READ; }
"write"		{update_col(yyleng); return TOKEN_WRITE; }
"true"		{update_col(yyleng); return TOKEN_TRUE; }
"false"		{update_col(yyleng); return TOKEN_FALSE; }
"or"		{update_col(yyleng); return TOKEN_OR; }
"not"		{update_col(yyleng); return TOKEN_NOT; }
"div"		{update_col(yyleng); return TOKEN_DIV; }

{id}		{update_col(yyleng); return TOKEN_ID; }
{numero}	{update_col(yyleng); return TOKEN_NUM; }

":="  		{update_col(yyleng); return TOKEN_ASSIGN; }
"<>"		{update_col(yyleng); return TOKEN_NEQ; }
"<"         { update_col(yyleng); return TOKEN_LT; }
"<="		{update_col(yyleng); return TOKEN_LTE; }
">"         { update_col(yyleng); return TOKEN_GT; }
">="		{update_col(yyleng); return TOKEN_GTE; }	

"+"|"-"|"*"|"="|"("|")"|";"|","|":"|"." { update_col(yyleng); return yytext[0]; }

[ \t]+		{update_col(yyleng); }
\n			{update_line(); }

<<EOF>> 	{return 0; }

.	{
		std::cerr << "Erro na linha " << line_num << ", coluna " << col_num << ": caractere inesperado " << yytext[0] << std::endl;
		update_col(yyleng);
		return yytext[0];
}

%%

int line_num = 1;
int col_num = 1;

void update_line() {
		line_num++;
		col_num = 1;
}

void update_col(int length) {
		col_num += length;
}
