#pragma once

#include "ASTVisitor.hpp"
#include "ASTNodes.hpp"
#include <sstream>
#include <map>
#include <vector>
#include <string>

struct SymbolInfo {
    int level;
    int offset;
    std::string type; // "variable", "parameter", "procedure", "function"
    std::string label; // For procedures/functions
    int numParams; // For procedures/functions
};

class GenSymbolTable {
public:
    void enterScope() {
        scopes.push_back({});
    }

    void exitScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    bool insert(const std::string& name, int level, int offset, const std::string& type, const std::string& label = "", int numParams = 0) {
        if (scopes.empty()) return false;
        auto& currentScope = scopes.back();
        if (currentScope.find(name) != currentScope.end()) {
            return false;
        }
        currentScope[name] = {level, offset, type, label, numParams};
        return true;
    }

    SymbolInfo* lookup(const std::string& name) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return &found->second;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::map<std::string, SymbolInfo>> scopes;
};

class CodeGenerator : public Visitor {
public:
    CodeGenerator();
    std::string getCode() const;

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

private:
    std::stringstream code;
    GenSymbolTable symbolTable;
    int currentLevel;
    int labelCount;
    int varOffset;

    std::string newLabel();
    void emit(const std::string& instr);
    void emit(const std::string& instr, int arg);
    void emit(const std::string& instr, int arg1, int arg2);
    void emit(const std::string& instr, const std::string& label);
    void emit(const std::string& instr, const std::string& label, int arg);
    void emitLabel(const std::string& label);
};
