#include "ASTPrinter.hpp"

void ASTPrinter::indent() {
    for (int i = 0; i < indentLevel; ++i) std::cout << "  ";
}

void ASTPrinter::visit(WhileStmt& node) {
    indent();
    std::cout << "WhileStmt (cmd_repetição)\n";
    indentLevel++;
    
    indent();
    std::cout << "Condition:\n";
    node.condition->accept(*this);
    
    indent();
    std::cout << "Body:\n";
    node.body->accept(*this);
    
    indentLevel--;
}

void ASTPrinter::visit(CompoundStmt& node) {
    indent();
    std::cout << "CompoundStmt (seq_comandos)\n"; 
    indentLevel++;
    
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
    
    indentLevel--;
}

void ASTPrinter::visit(AssignStmt& node) {
    indent();
    std::cout << "AssignStmt (cmd_atrib: " << node.varName << ")\n";
    indentLevel++;
    
    indent();
    std::cout << "Expression:\n";
    node.expression->accept(*this);
    
    indentLevel--;
}

void ASTPrinter::visit(BinaryExpr& node) {
    indent();
    std::cout << "BinaryExpr (exp_binária: '" << node.op << "')\n";
    indentLevel++;
    
    indent();
    std::cout << "LHS:\n";
    node.left->accept(*this);
    
    indent();
    std::cout << "RHS:\n";
    node.right->accept(*this);
    
    indentLevel--;
}

void ASTPrinter::visit(VarExpr& node) {
    indent();
    std::cout << "VarExpr (exp_var: " << node.varName << ")\n";
}

void ASTPrinter::visit(NumberExpr& node) {
    indent();
    std::cout << "NumberExpr (exp_num: " << node.value << ")\n";
}


void ASTPrinter::visit(ProgramNode& node) { indent(); std::cout << "ProgramNode\n"; }
void ASTPrinter::visit(BlockNode& node) { indent(); std::cout << "BlockNode\n"; }
void ASTPrinter::visit(VarDecl& node) { indent(); std::cout << "VarDecl\n"; }
void ASTPrinter::visit(ProcDecl& node) { indent(); std::cout << "ProcDecl\n"; }
void ASTPrinter::visit(FuncDecl& node) { indent(); std::cout << "FuncDecl\n"; }
void ASTPrinter::visit(ParamDecl& node) { indent(); std::cout << "ParamDecl\n"; }
void ASTPrinter::visit(TypeNode& node) { indent(); std::cout << "TypeNode\n"; }
void ASTPrinter::visit(IfStmt& node) { indent(); std::cout << "IfStmt\n"; }
void ASTPrinter::visit(ProcCallStmt& node) { indent(); std::cout << "ProcCallStmt\n"; }
void ASTPrinter::visit(ReadStmt& node) { indent(); std::cout << "ReadStmt\n"; }
void ASTPrinter::visit(WriteStmt& node) { indent(); std::cout << "WriteStmt\n"; }
void ASTPrinter::visit(UnaryExpr& node) { indent(); std::cout << "UnaryExpr\n"; }
void ASTPrinter::visit(BoolExpr& node) { indent(); std::cout << "BoolExpr\n"; }
void ASTPrinter::visit(CallExpr& node) { indent(); std::cout << "CallExpr\n"; }