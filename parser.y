%{
#include <iostream>
#include <cstdio>
#include "wrapper.h"

extern FILE* yyin;

void yyerror(const char* s) {
    std::cerr << "Erro de sintaxe, linha: " << line_num << " col: " << col_num << " - " << s << std::endl;
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

// Precedência e associatividade
%left TOKEN_OR
%left TOKEN_AND
%left '=' TOKEN_NEQ '<' TOKEN_LTE '>' TOKEN_GTE
%left '+' '-'
%left '*' TOKEN_DIV
%right TOKEN_NOT
%right UMINUS
%nonassoc TOKEN_THEN
%nonassoc TOKEN_ELSE

// Regras da Gramática
%%

program:
    TOKEN_PROGRAM TOKEN_ID ';' bloco '.'
    ;

bloco:
    comando_composto
    | secao_declaracao_variaveis comando_composto
    | secao_declaracao_subrotinas comando_composto
    | secao_declaracao_variaveis secao_declaracao_subrotinas comando_composto
    ;

secao_declaracao_variaveis:
    TOKEN_VAR declaracao_variaveis_lista
    ;

declaracao_variaveis_lista:
    declaracao_variaveis ';'
    | declaracao_variaveis_lista declaracao_variaveis ';'
    ;

declaracao_variaveis:
    lista_identificador ':' tipo
    ;

lista_identificador:
    TOKEN_ID
    | lista_identificador ',' TOKEN_ID
    ;

tipo:
    TOKEN_BOOLEAN
    | TOKEN_INTEGER
    ;

secao_declaracao_subrotinas:
    declaracao_procedimento ';'
    | declaracao_funcao ';'
    | secao_declaracao_subrotinas declaracao_procedimento ';'
    | secao_declaracao_subrotinas declaracao_funcao ';'
    ;

declaracao_procedimento:
    TOKEN_PROCEDURE TOKEN_ID ';' bloco_subrot
    | TOKEN_PROCEDURE TOKEN_ID parametros_formais ';' bloco_subrot
    ;

declaracao_funcao:
    TOKEN_FUNCTION TOKEN_ID ':' tipo ';' bloco_subrot
    | TOKEN_FUNCTION TOKEN_ID parametros_formais ':' tipo ';' bloco_subrot
    ;

bloco_subrot:
    comando_composto
    | secao_declaracao_variaveis comando_composto
    ;

parametros_formais:
    '(' declaracao_parametros_lista ')'
    ;

declaracao_parametros_lista:
    declaracao_parametros
    | declaracao_parametros_lista ';' declaracao_parametros
    ;

declaracao_parametros:
    lista_identificador ':' tipo
    ;

comando_composto:
    TOKEN_BEGIN comando_lista TOKEN_END
    ;

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
    TOKEN_ID '(' lista_expressao_opcional ')'
    ;

condicional:
    TOKEN_IF expressao TOKEN_THEN comando
    | TOKEN_IF expressao TOKEN_THEN comando TOKEN_ELSE comando
    ;

repeticao:
    TOKEN_WHILE expressao TOKEN_DO comando
    ;

leitura:
    TOKEN_READ '(' lista_identificador ')'
    ;

escrita:
    TOKEN_WRITE '(' lista_expressao ')'
    ;

lista_expressao:
    expressao
    | lista_expressao ',' expressao
    ;

expressao:
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
    | expressao_simples '-' termo
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
    | '-' fator %prec UMINUS
    ;

variavel:
    TOKEN_ID
    ;

logico:
    TOKEN_FALSE
    | TOKEN_TRUE
    ;

chamada_funcao:
    TOKEN_ID '(' lista_expressao_opcional ')'
    ;

lista_expressao_opcional:
    /* empty */
    | lista_expressao
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
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return result;
}
