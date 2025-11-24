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


void ASTPrinter::visit(ProgramNode& node) {
    indent();
    std::cout << "ProgramNode (" << node.programName << ")\n";
    indentLevel++;
    node.block->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(BlockNode& node) {
    indent();
    std::cout << "BlockNode\n";
    indentLevel++;
    for (auto& var : node.varDeclarations) var->accept(*this);
    for (auto& sub : node.subroutineDeclarations) sub->accept(*this);
    node.compoundStatement->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(VarDecl& node) {
    indent();
    std::cout << "VarDecl: ";
    for (size_t i = 0; i < node.idList.size(); ++i) {
        std::cout << node.idList[i] << (i < node.idList.size() - 1 ? ", " : "");
    }
    std::cout << " : " << node.type->typeName << "\n";
}

void ASTPrinter::visit(ProcDecl& node) {
    indent();
    std::cout << "ProcDecl (" << node.procName << ")\n";
    indentLevel++;
    for (auto& param : node.params) param->accept(*this);
    node.block->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(FuncDecl& node) {
    indent();
    std::cout << "FuncDecl (" << node.funcName << ")\n";
    indentLevel++;
    for (auto& param : node.params) param->accept(*this);
    std::cout << "Return Type: " << node.returnType->typeName << "\n";
    node.block->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(ParamDecl& node) {
    indent();
    std::cout << "ParamDecl: ";
    for (size_t i = 0; i < node.idList.size(); ++i) {
        std::cout << node.idList[i] << (i < node.idList.size() - 1 ? ", " : "");
    }
    std::cout << " : " << node.type->typeName << "\n";
}

void ASTPrinter::visit(TypeNode& node) {
    indent();
    std::cout << "TypeNode (" << node.typeName << ")\n";
}

void ASTPrinter::visit(IfStmt& node) {
    indent();
    std::cout << "IfStmt\n";
    indentLevel++;
    indent(); std::cout << "Condition:\n";
    node.condition->accept(*this);
    indent(); std::cout << "Then:\n";
    node.thenStmt->accept(*this);
    if (node.elseStmt) {
        indent(); std::cout << "Else:\n";
        node.elseStmt->accept(*this);
    }
    indentLevel--;
}

void ASTPrinter::visit(ProcCallStmt& node) {
    indent();
    std::cout << "ProcCallStmt (" << node.procName << ")\n";
    indentLevel++;
    for (auto& arg : node.arguments) arg->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(ReadStmt& node) {
    indent();
    std::cout << "ReadStmt\n";
    indentLevel++;
    for (auto& var : node.variables) var->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(WriteStmt& node) {
    indent();
    std::cout << "WriteStmt\n";
    indentLevel++;
    for (auto& expr : node.expressions) expr->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(UnaryExpr& node) {
    indent();
    std::cout << "UnaryExpr (" << node.op << ")\n";
    indentLevel++;
    node.right->accept(*this);
    indentLevel--;
}

void ASTPrinter::visit(BoolExpr& node) {
    indent();
    std::cout << "BoolExpr (" << (node.value ? "true" : "false") << ")\n";
}

void ASTPrinter::visit(CallExpr& node) {
    indent();
    std::cout << "CallExpr (" << node.funcName << ")\n";
    indentLevel++;
    for (auto& arg : node.arguments) arg->accept(*this);
    indentLevel--;
}