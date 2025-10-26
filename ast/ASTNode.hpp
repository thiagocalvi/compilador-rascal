#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

// Nós concretos e Visitor
class Visitor;
class ProgramNode;
class BlockNode;
class VarDecl;
class ProcDecl;
class FuncDecl;
class ParamDecl;
class CompoundStmt;
class AssignStmt;
class IfStmt;
class WhileStmt;
class ProcCallStmt;
class ReadStmt;
class WriteStmt;
class BinaryExpr;
class UnaryExpr;
class NumberExpr;
class BoolExpr;
class VarExpr;
class CallExpr;
class TypeNode;

// Ponteiro inteligente para nós da AST
template<typename T>
using NodePtr = std::unique_ptr<T>;

// Classe base para todos os nós da AST
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// Categorias de nós
class Expr : public ASTNode {}; // Expressões (5, a + b)
class Stmt : public ASTNode {}; // Comandos (while, if, :=)
class Decl : public ASTNode {}; // Declarações (var, procedure)