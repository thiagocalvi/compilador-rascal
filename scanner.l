%{
#include <iostream>
#include "tokens.hpp"
}%

%option c++ //Gerar scanner em C++

int line_num = 1;
int col_num = 1;

void update_line() {
		line_num++;
		col_num = 1;
}

void update_col() {
		col_num += yyleng;
}

letra	[a-zA-Z]
digito	[0-9]
id		{letra}({letra}|{digito}|_)
numero	{digito}+

%%
"program"	{update_col(); return TOKEN_PROGRAM; }
"var"		{update_col(); return TOKEN_VAR; }
"integer"
"boolean"
"procedure"
"function"
"begin"
"end"
"if"

[\t]+		{update_col(); }
\n			{update_line(); }
%%
