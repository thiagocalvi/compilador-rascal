%{
#include <iostream>
#include <string.h>
#include "wrapper.h"

extern FILE* yyin;

void yyerror(const char* s) {
		std::cerr << "Erro de sintaxe, linha: " << line_num << "col: " << col_num << " - " << s << std::endl;
};

%}

// Tokens
%token TOKEN_PROGRAM
%token TOKEN_VAR
%token TOKEN_INTEGER
%token TOKEN_BOOLEAN
%token TOKEN_PROCEDURE
%token TOKEN_FUNCTION
%token TOKEN_BEGIN
%token TOKEN_END
%token TOKEN_IF
%token TOKEN_THEN
%token TOKEN_ELSE
%token TOKEN_WHILE
%token TOKEN_DO
%token TOKEN_READ
%token TOKEN_WRITE
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_OR
%token TOKEN_NOT
%token TOKEN_DIV

%token TOKEN_ID
%token TOKEN_NUM

%token TOKEN_ASSIGN
%token TOKEN_NEQ
%token TOKEN_LT
%token TOKEN_LTE
%token TOKEN_GT
%token TOKEN_GTE

%token '+' '-' '*' '=' '(' ')' ';' ',' ':' '.'

// Regras da Gramática
%%
program:
		TOKEN_PROGRAM TOKEN_ID ';' bloco '.'
		{
        	std::cout << "Programa reconhecido!" << std::endl;
    	}
    	;

bloco:
		secao_declaracao_variaveis
		| secao_declaracao_subrotinas
		| comando_composto
		;

secao_declaracao_variaveis:
		TOKEN_VAR declaracao_variaveis ';'
		| declaracao_variaveis ';'
		;

declaracao_variaveis:
		lista_identificadores ':' tipo
		;

lista_identificadores:
		
tipo:
		TOKEN_BOOLEAN | TOKEN_INTEGER
		;

secao_declaracao_subrotinas:
declaracao_procedimento:
declaracao_funcao:
bloco_subrot:
parametros_formais:
declaracao_parametros:
comando_composto:
comando:
atribuicao:
chamada_procedimento:
condicional:
repeticao:
leitura:
escrita:
lista_expressoes:
expressoes:
relacao:
expressao_simples:
termo:
fator:
variavel:
logico:
chama_funcao:

%%

int main(int argc, char** argv) {
		if (argc > 1) {
				yyin = fopen(argv[1], "r");
				if (!yyin) {
						std::cerr << "Erro ao abrir arquivo: " << argv[1] << std::endl;
						return 1;
				}

		} else {
				yyin = stdin;
		}

		int result = yyparse();
		fclose(yyin);
		return result;
}
