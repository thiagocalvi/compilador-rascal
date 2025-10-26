%{
#include <iostream>
#include <string.h>
#include "wrapper.h"

extern FILE* yyin;

void yyerror(const char* s) {
		std::cerr << "Erro de sintaxe, linha: " << line_num << "col: " << col_num << " - " << s << std::endl;
};

%}

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
%token TOKEN_AND
%token TOKEN_NOT
%token TOKEN_DIV

%token TOKEN_ID
%token TOKEN_NUM

%token TOKEN_ASSIGN
%token TOKEN_NEQ
%token TOKEN_LTE
%token TOKEN_GTE

%token '+' '-' '*' '=' '(' ')' ';' ',' ':' '.' '<' '>'

// Precedência e associatividade
%left TOKEN_OR
%left TOKEN_AND
%left '=' TOKEN_NEQ '<' TOKEN_LTE '>' TOKEN_GTE
%left '+' '-'
%left '*' TOKEN_DIV
%right TOKEN_NOT
%right UMINUS


// Regras da Gramática
%%
program:
		TOKEN_PROGRAM TOKEN_ID ';' bloco '.'
    	;

bloco:
		secao_declaracao_variaveis secao_declaracao_subrotinas comando_composto
		;

secao_declaracao_variveis_opt:
		
		| secao_declaracao_variaveis
		;

secao_declaracao_variaveis:
		TOKEN_VAR declaracao_variaveis ';'
		| declaracao_variaveis ';'
		;

declaracao_variaveis:
		lista_identificadores ':' tipo
		;

lista_identificadores:
					 TOKEN_ID
					 | lista_identificadores ',' TOKEN_ID
					 ;
		
tipo:
	TOKEN_BOOLEAN 
	| TOKEN_INTEGER
	;

secao_declaracao_subrotinas:
declaracao_procedimento:
declaracao_funcao:
bloco_subrot:
parametros_formais:
declaracao_parametros:
		lista_identificadores ':' tipo
		;

comando_composto:
		TOKEN_BEGIN comando_lista TOKEN_END

comando_lista:
		comando
		| comando_lista ';' comando
		;

comando:
	   atribuicao
	   | chamada_procedimento
	   | condicional
	   | repeticao
	   | leitura
	   | escrita
	   | comando_composto
	   ;

atribuicao:
		TOKEN_ID TOKEN_ASSIGN expressao
		;

chamada_procedimento:
		TOKEN_ID
		| TOKEN_ID '(' lista_expressoes ')'

condicional:
		TOKEN_IF expressao TOKEN_THEN comando 
		| TOKEN_IF expressap TOKEN_THEN comando TOKEN_ELSE comando
		;

repeticao:
		TOKEN_WHILE expressao TOKEN_DO comando
		;

leitura:
		TOKEN_READ '(' lista_identificadores ')'
		;

escrita:
		TOKEN_WRITE '(' lista_expressoes ')'
		;

lista_expressoes:
		expressao
		| lista_expressoes ',' expressao
		;

expressoes:
		expressao_simples
		| expressao_simples relacao expressao_simples
		;

relacao:
		'='
		| TOKEN_NEQ
		| '<'
		| TOKEN_LTE
		| '>'
		| TOKEN_GTE
		;

expressao_simples:
		termo
		| expressao_simples '+' termo
		| expressao_simpels '-' termo
		| expressao_simples TOKEN_OR termo
		;
 
termo:
		fator
		| termo '*' fator
		| termo TOKEN_DIV fator
		| termo TOKEN_AND fator
		;

fator:
		variavel
		| TOKEN_NUM
		| logico
		| chamada_funcao
		| '(' expressao ')'
		| TOKEN_NOT fator
		//| '-' fator %prec UMINUS
		;

variavel:
		TOKEN_ID
		;

logico:
		TOKEN_FALSE
		| TOKEN_TRUE
		;

chamada_funcao:
			TOKEN_ID '(' lista_expressoes ')'
			;

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
