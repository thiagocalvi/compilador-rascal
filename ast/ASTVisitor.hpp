#pragma once

#include "ASTNode.hpp"

// visitor base class
class Visitor {
public:
    virtual ~Visitor() = default;

    // main nodes
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(BlockNode& node) = 0;

    // declarations
    virtual void visit(VarDecl& node) = 0;
    virtual void visit(ProcDecl& node) = 0;
    virtual void visit(FuncDecl& node) = 0;
    virtual void visit(ParamDecl& node) = 0;
    virtual void visit(TypeNode& node) = 0;

    // statements
    virtual void visit(CompoundStmt& node) = 0;
    virtual void visit(AssignStmt& node) = 0;
    virtual void visit(IfStmt& node) = 0;
    virtual void visit(WhileStmt& node) = 0;
    virtual void visit(ProcCallStmt& node) = 0;
    virtual void visit(ReadStmt& node) = 0;
    virtual void visit(WriteStmt& node) = 0;

    // expressions
    virtual void visit(BinaryExpr& node) = 0;
    virtual void visit(UnaryExpr& node) = 0;
    virtual void visit(NumberExpr& node) = 0;
    virtual void visit(BoolExpr& node) = 0;
    virtual void visit(VarExpr& node) = 0;
    virtual void visit(CallExpr& node) = 0;
};