#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

// Concrete nodes and visitor
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

// Smart pointer for AST nodes
template<typename T>
using NodePtr = std::unique_ptr<T>;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// Categories of nodes
class Expr : public ASTNode {}; // Expressions (5, a + b)
class Stmt : public ASTNode {}; // Commands (while, if, :=)
class Decl : public ASTNode {}; // Declarations (var, procedure)