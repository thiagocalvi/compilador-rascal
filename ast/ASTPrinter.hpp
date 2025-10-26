#pragma once

#include "ASTNodes.hpp"
#include <iostream>

class ASTPrinter : public Visitor {
private:
    int indentLevel = 0;
    void indent();

public:
    void visit(ProgramNode& node) override;
    void visit(BlockNode& node) override;
    void visit(VarDecl& node) override;
    void visit(ProcDecl& node) override;
    void visit(FuncDecl& node) override;
    void visit(ParamDecl& node) override;
    void visit(TypeNode& node) override;
    void visit(CompoundStmt& node) override;
    void visit(AssignStmt& node) override;
    void visit(IfStmt& node) override;
    void visit(WhileStmt& node) override;
    void visit(ProcCallStmt& node) override;
    void visit(ReadStmt& node) override;
    void visit(WriteStmt& node) override;
    void visit(BinaryExpr& node) override;
    void visit(UnaryExpr& node) override;
    void visit(NumberExpr& node) override;
    void visit(BoolExpr& node) override;
    void visit(VarExpr& node) override;
    void visit(CallExpr& node) override;
};