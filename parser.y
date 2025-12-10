%{
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "wrapper.h"
#include "ast/ASTPrinter.hpp"
#include "semantic/SemanticChecker.hpp"
#include "ast/CodeGenerator.hpp"
#include <fstream>

extern FILE* yyin;

void yyerror(const char* s) {
    error_count++;
    std::cerr << "ERRO SINTÁTICO [linha " << line_num << ", coluna " << col_num << "]: " << s << std::endl;

};

%}

%code requires {
    #include "ast/ASTNodes.hpp"
}

%union {
    int intVal;
    char* strVal;
    ASTNode* node;
    ProgramNode* program;
    BlockNode* block;
    Stmt* stmt;
    Expr* expr;
    Decl* decl;
    TypeNode* type;
    VarDecl* varDecl;
    std::vector<NodePtr<Stmt>>* stmtList;
    std::vector<NodePtr<Expr>>* exprList;
    std::vector<std::string>* idList;
    std::vector<NodePtr<VarDecl>>* varDeclList;
    std::vector<NodePtr<ASTNode>>* subroutList;
    std::vector<NodePtr<ParamDecl>>* paramList;
    ParamDecl* paramDecl;
}

%define parse.error verbose

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
%token <strVal> TOKEN_ID
%token <intVal> TOKEN_NUM
%token TOKEN_ASSIGN
%token TOKEN_NEQ
%token TOKEN_LTE
%token TOKEN_GTE

%type <program> program
%type <block> bloco
%type <stmt> comando comando_composto atribuicao chamada_procedimento condicional repeticao leitura escrita
%type <stmtList> comando_lista
%type <expr> expressao expressao_simples termo fator variavel logico chamada_funcao
%type <exprList> lista_expressao lista_expressao_opcional
%type <type> tipo
%type <idList> lista_identificador
%type <varDecl> declaracao_variaveis
%type <varDeclList> secao_declaracao_variaveis declaracao_variaveis_lista
%type <subroutList> secao_declaracao_subrotinas
%type <decl> declaracao_procedimento declaracao_funcao
%type <block> bloco_subrot
%type <paramList> parametros_formais declaracao_parametros_lista
%type <paramDecl> declaracao_parametros

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
    TOKEN_PROGRAM TOKEN_ID ';' bloco '.' {
        $$ = new ProgramNode($2, NodePtr<BlockNode>($4));
        extern ProgramNode* root;
        root = $$;
    }
    ;

bloco:
    comando_composto {
        $$ = new BlockNode(
            std::vector<NodePtr<VarDecl>>(),
            std::vector<NodePtr<ASTNode>>(),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($1))
        );
    }
    | secao_declaracao_variaveis comando_composto {
        std::vector<NodePtr<VarDecl>> vars;
        for(auto& v : *$1) vars.push_back(std::move(v));
        delete $1;
        $$ = new BlockNode(
            std::move(vars),
            std::vector<NodePtr<ASTNode>>(),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($2))
        );
    }
    | secao_declaracao_subrotinas comando_composto {
        std::vector<NodePtr<ASTNode>> subs;
        for(auto& s : *$1) subs.push_back(std::move(s));
        delete $1;
        $$ = new BlockNode(
            std::vector<NodePtr<VarDecl>>(),
            std::move(subs),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($2))
        );
    }
    | secao_declaracao_variaveis secao_declaracao_subrotinas comando_composto {
        std::vector<NodePtr<VarDecl>> vars;
        for(auto& v : *$1) vars.push_back(std::move(v));
        delete $1;
        std::vector<NodePtr<ASTNode>> subs;
        for(auto& s : *$2) subs.push_back(std::move(s));
        delete $2;
        $$ = new BlockNode(
            std::move(vars),
            std::move(subs),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($3))
        );
    }
    ;

secao_declaracao_variaveis:
    TOKEN_VAR declaracao_variaveis_lista { $$ = $2; }
    ;

declaracao_variaveis_lista:
    declaracao_variaveis ';' {
        $$ = new std::vector<NodePtr<VarDecl>>();
        $$->push_back(NodePtr<VarDecl>($1));
    }
    | declaracao_variaveis_lista declaracao_variaveis ';' {
        $$ = $1;
        $$->push_back(NodePtr<VarDecl>($2));
    }
    | error ';' { yyerrok; $$ = new std::vector<NodePtr<VarDecl>>(); }
    | declaracao_variaveis_lista error ';' { yyerrok; $$ = $1; }
    ;

declaracao_variaveis:
    lista_identificador ':' tipo {
        $$ = new VarDecl(*$1, NodePtr<TypeNode>($3));
        delete $1;
    }
    ;

lista_identificador:
    TOKEN_ID {
        $$ = new std::vector<std::string>();
        $$->push_back($1);
    }
    | lista_identificador ',' TOKEN_ID {
        $$ = $1;
        $$->push_back($3);
    }
    ;

tipo:
    TOKEN_BOOLEAN { $$ = new TypeNode("boolean"); }
    | TOKEN_INTEGER { $$ = new TypeNode("integer"); }
    ;

secao_declaracao_subrotinas:
    declaracao_procedimento ';' {
        $$ = new std::vector<NodePtr<ASTNode>>();
        $$->push_back(NodePtr<ASTNode>($1));
    }
    | declaracao_funcao ';' {
        $$ = new std::vector<NodePtr<ASTNode>>();
        $$->push_back(NodePtr<ASTNode>($1));
    }
    | secao_declaracao_subrotinas declaracao_procedimento ';' {
        $$ = $1;
        $$->push_back(NodePtr<ASTNode>($2));
    }
    | secao_declaracao_subrotinas declaracao_funcao ';' {
        $$ = $1;
        $$->push_back(NodePtr<ASTNode>($2));
    }
    | error ';' { yyerrok; $$ = new std::vector<NodePtr<ASTNode>>(); }
    | secao_declaracao_subrotinas error ';' { yyerrok; $$ = $1; }
    ;

declaracao_procedimento:
    TOKEN_PROCEDURE TOKEN_ID ';' bloco_subrot {
        $$ = new ProcDecl($2, std::vector<NodePtr<ParamDecl>>(), NodePtr<BlockNode>($4));
    }
    | TOKEN_PROCEDURE TOKEN_ID parametros_formais ';' bloco_subrot {
        std::vector<NodePtr<ParamDecl>> params;
        for(auto& p : *$3) params.push_back(std::move(p));
        delete $3;
        $$ = new ProcDecl($2, std::move(params), NodePtr<BlockNode>($5));
    }
    ;

declaracao_funcao:
    TOKEN_FUNCTION TOKEN_ID ':' tipo ';' bloco_subrot {
        $$ = new FuncDecl($2, std::vector<NodePtr<ParamDecl>>(), NodePtr<TypeNode>($4), NodePtr<BlockNode>($6));
    }
    | TOKEN_FUNCTION TOKEN_ID parametros_formais ':' tipo ';' bloco_subrot {
        std::vector<NodePtr<ParamDecl>> params;
        for(auto& p : *$3) params.push_back(std::move(p));
        delete $3;
        $$ = new FuncDecl($2, std::move(params), NodePtr<TypeNode>($5), NodePtr<BlockNode>($7));
    }
    ;

bloco_subrot:
    comando_composto {
        $$ = new BlockNode(
            std::vector<NodePtr<VarDecl>>(),
            std::vector<NodePtr<ASTNode>>(),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($1))
        );
    }
    | secao_declaracao_variaveis comando_composto {
        std::vector<NodePtr<VarDecl>> vars;
        for(auto& v : *$1) vars.push_back(std::move(v));
        delete $1;
        $$ = new BlockNode(
            std::move(vars),
            std::vector<NodePtr<ASTNode>>(),
            NodePtr<CompoundStmt>(static_cast<CompoundStmt*>($2))
        );
    }
    ;

parametros_formais:
    '(' declaracao_parametros_lista ')' { $$ = $2; }
    ;

declaracao_parametros_lista:
    declaracao_parametros {
        $$ = new std::vector<NodePtr<ParamDecl>>();
        $$->push_back(NodePtr<ParamDecl>($1));
    }
    | declaracao_parametros_lista ';' declaracao_parametros {
        $$ = $1;
        $$->push_back(NodePtr<ParamDecl>($3));
    }
    ;

declaracao_parametros:
    lista_identificador ':' tipo {
        $$ = new ParamDecl(*$1, NodePtr<TypeNode>($3));
        delete $1;
    }
    ;

comando_composto:
    TOKEN_BEGIN comando_lista TOKEN_END {
        std::vector<NodePtr<Stmt>> stmts;
        for(auto& s : *$2) stmts.push_back(std::move(s));
        delete $2;
        $$ = new CompoundStmt(std::move(stmts));
    }
    ;

comando_lista:
    comando {
        $$ = new std::vector<NodePtr<Stmt>>();
        $$->push_back(NodePtr<Stmt>($1));
    }
    | comando_lista ';' comando {
        $$ = $1;
        $$->push_back(NodePtr<Stmt>($3));
    }
    | error { $$ = new std::vector<NodePtr<Stmt>>(); }
    | comando_lista ';' error { $$ = $1; }
    ;

comando:
    atribuicao { $$ = $1; }
    | chamada_procedimento { $$ = $1; }
    | condicional { $$ = $1; }
    | repeticao { $$ = $1; }
    | leitura { $$ = $1; }
    | escrita { $$ = $1; }
    | comando_composto { $$ = $1; }
    ;

atribuicao:
    TOKEN_ID TOKEN_ASSIGN expressao {
        $$ = new AssignStmt($1, NodePtr<Expr>($3));
    }
    | TOKEN_ID error expressao { yyerrok; $$ = new AssignStmt($1, NodePtr<Expr>($3)); } // Recuperação simples
    ;

chamada_procedimento:
    TOKEN_ID '(' lista_expressao_opcional ')' {
        std::vector<NodePtr<Expr>> args;
        for(auto& a : *$3) args.push_back(std::move(a));
        delete $3;
        $$ = new ProcCallStmt($1, std::move(args));
    }
    ;

condicional:
    TOKEN_IF expressao TOKEN_THEN comando {
        $$ = new IfStmt(NodePtr<Expr>($2), NodePtr<Stmt>($4), nullptr);
    }
    | TOKEN_IF expressao TOKEN_THEN comando TOKEN_ELSE comando {
        $$ = new IfStmt(NodePtr<Expr>($2), NodePtr<Stmt>($4), NodePtr<Stmt>($6));
    }
    ;

repeticao:
    TOKEN_WHILE expressao TOKEN_DO comando {
        $$ = new WhileStmt(NodePtr<Expr>($2), NodePtr<Stmt>($4));
    }
    ;

leitura:
    TOKEN_READ '(' lista_identificador ')' {
        std::vector<NodePtr<VarExpr>> vars;
        for(const auto& id : *$3) vars.push_back(std::make_unique<VarExpr>(id));
        delete $3;
        $$ = new ReadStmt(std::move(vars));
    }
    ;

escrita:
    TOKEN_WRITE '(' lista_expressao ')' {
        std::vector<NodePtr<Expr>> exprs;
        for(auto& e : *$3) exprs.push_back(std::move(e));
        delete $3;
        $$ = new WriteStmt(std::move(exprs));
    }
    ;

lista_expressao:
    expressao {
        $$ = new std::vector<NodePtr<Expr>>();
        $$->push_back(NodePtr<Expr>($1));
    }
    | lista_expressao ',' expressao {
        $$ = $1;
        $$->push_back(NodePtr<Expr>($3));
    }
    ;

expressao:
    expressao_simples { $$ = $1; }
    ;

expressao:
      //expressao_simples { $$ = $1; }
    expressao_simples '=' expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), "=", NodePtr<Expr>($3)); }
    | expressao_simples TOKEN_NEQ expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), "<>", NodePtr<Expr>($3)); }
    | expressao_simples '<' expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), "<", NodePtr<Expr>($3)); }
    | expressao_simples TOKEN_LTE expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), "<=", NodePtr<Expr>($3)); }
    | expressao_simples '>' expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), ">", NodePtr<Expr>($3)); }
    | expressao_simples TOKEN_GTE expressao_simples { $$ = new BinaryExpr(NodePtr<Expr>($1), ">=", NodePtr<Expr>($3)); }
    ;

expressao_simples:
    termo { $$ = $1; }
    | expressao_simples '+' termo { $$ = new BinaryExpr(NodePtr<Expr>($1), "+", NodePtr<Expr>($3)); }
    | expressao_simples '-' termo { $$ = new BinaryExpr(NodePtr<Expr>($1), "-", NodePtr<Expr>($3)); }
    | expressao_simples TOKEN_OR termo { $$ = new BinaryExpr(NodePtr<Expr>($1), "or", NodePtr<Expr>($3)); }
    ;

termo:
    fator { $$ = $1; }
    | termo '*' fator { $$ = new BinaryExpr(NodePtr<Expr>($1), "*", NodePtr<Expr>($3)); }
    | termo TOKEN_DIV fator { $$ = new BinaryExpr(NodePtr<Expr>($1), "div", NodePtr<Expr>($3)); }
    | termo TOKEN_AND fator { $$ = new BinaryExpr(NodePtr<Expr>($1), "and", NodePtr<Expr>($3)); }
    ;

fator:
    variavel { $$ = $1; }
    | TOKEN_NUM { $$ = new NumberExpr($1); }
    | logico { $$ = $1; }
    | chamada_funcao { $$ = $1; }
    | '(' expressao ')' { $$ = $2; }
    | '(' error ')' { yyerrok; $$ = new NumberExpr(0); } // Dummy error node
    | TOKEN_NOT fator { $$ = new UnaryExpr("not", NodePtr<Expr>($2)); }
    | '-' fator %prec UMINUS { $$ = new UnaryExpr("-", NodePtr<Expr>($2)); }
    ;

variavel:
    TOKEN_ID { $$ = new VarExpr($1); }
    ;

logico:
    TOKEN_FALSE { $$ = new BoolExpr(false); }
    | TOKEN_TRUE { $$ = new BoolExpr(true); }
    ;

chamada_funcao:
    TOKEN_ID '(' lista_expressao_opcional ')' {
        std::vector<NodePtr<Expr>> args;
        for(auto& a : *$3) args.push_back(std::move(a));
        delete $3;
        $$ = new CallExpr($1, std::move(args));
    }
    ;

lista_expressao_opcional:
    /* empty */ { $$ = new std::vector<NodePtr<Expr>>(); }
    | lista_expressao { $$ = $1; }
    ;

%%

ProgramNode* root = nullptr;

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

    // Output file name
    std::string outFileName = "output.mepa"; // fallback
    if (argc > 2) {
        outFileName = argv[2];
    }

    std::cout << "=== Iniciando compilação ===" << std::endl;
    
    int result = yyparse();
    
    if (yyin != stdin) {
        fclose(yyin);
    }

    std::cout << "Total de erros encontrados: " << error_count << std::endl;
    
    if (result == 0 && error_count == 0 && root != nullptr) {
        std::cout << "\n=== AST Gerada ===\n";
        ASTPrinter printer;
        root->accept(printer);

        std::cout << "\n=== Análise Semântica ===\n";
        SemanticChecker checker;
        root->accept(checker);

        if (checker.getErrorCount() > 0) {
             std::cout << "Total de erros semânticos: " << checker.getErrorCount() << std::endl;
        } else {
             std::cout << "Análise semântica concluída com sucesso." << std::endl;
             
             std::cout << "\n=== Geração de Código ===\n";
             CodeGenerator generator;
             root->accept(generator);
             
             std::string generatedCode = generator.getCode();
             std::cout << generatedCode;

             // Save to file
             std::ofstream outFile(outFileName);
             outFile << generatedCode;
             outFile.close();
             std::cout << "\nCódigo salvo em '" << outFileName << "'.\n";
        }

        delete root;
    }
    std::cout << "\n=== Processo finalizado ===" << std::endl;
    return result;
}
