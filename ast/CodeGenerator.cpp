#include "CodeGenerator.hpp"
#include <iostream>

CodeGenerator::CodeGenerator() : currentLevel(-1), labelCount(0), varOffset(0) {}

std::string CodeGenerator::getCode() const {
    return code.str();
}

std::string CodeGenerator::newLabel() {
    return "L" + std::to_string(labelCount++);
}

void CodeGenerator::emit(const std::string& instr) {
    code << "\t" << instr << "\n";
}

void CodeGenerator::emit(const std::string& instr, int arg) {
    code << "\t" << instr << " " << arg << "\n";
}

void CodeGenerator::emit(const std::string& instr, int arg1, int arg2) {
    code << "\t" << instr << " " << arg1 << "," << arg2 << "\n";
}

void CodeGenerator::emit(const std::string& instr, const std::string& label) {
    code << "\t" << instr << " " << label << "\n";
}

void CodeGenerator::emit(const std::string& instr, const std::string& label, int arg) {
    code << "\t" << instr << " " << label << "," << arg << "\n";
}

void CodeGenerator::emitLabel(const std::string& label) {
    code << label << ": " << "NADA" << "\n";
}

void CodeGenerator::visit(ProgramNode& node) {
    emit("INPP");
    currentLevel = 0; // Global level
    symbolTable.enterScope();
    node.block->accept(*this);
    symbolTable.exitScope();
    emit("PARA");
    emit("FIM"); // End of program marker
}

void CodeGenerator::visit(BlockNode& node) {
    // Count variables
    int numVars = 0;
    for (const auto& decl : node.varDeclarations) {
        numVars += decl->idList.size();
    }

    if (numVars > 0) {
        emit("AMEM", numVars);
    }

    // Register variables
    int localOffset = 0; // Start from 0 for current block vars
    
    for (const auto& decl : node.varDeclarations) {
        for (const auto& id : decl->idList) {
            symbolTable.insert(id, currentLevel, localOffset, "variable");
            localOffset++;
        }
    }

    // Visit subroutines
    for (const auto& sub : node.subroutineDeclarations) {
        sub->accept(*this);
    }

    // Visit statements
    node.compoundStatement->accept(*this);

    if (numVars > 0) {
        emit("DMEM", numVars);
    }
}

void CodeGenerator::visit(VarDecl& node) {
    // Already handled in BlockNode
}

void CodeGenerator::visit(ProcDecl& node) {
    std::string label = newLabel();
    std::string endLabel = newLabel();
    
    // Register procedure in current scope
    int numParams = 0;
    for (const auto& param : node.params) {
        numParams += param->idList.size();
    }
    symbolTable.insert(node.procName, currentLevel, 0, "procedure", label, numParams);

    emit("DSVS", endLabel);
    emitLabel(label);
    emit("ENPR", currentLevel + 1);

    currentLevel++;
    symbolTable.enterScope();

    // Register parameters
    // Last param is at -5. First param is at -5 - (n - 1).
    // We iterate params in order.
    int paramIndex = 0;
    for (const auto& param : node.params) {
        for (const auto& id : param->idList) {
            int offset = -5 - (numParams - 1 - paramIndex);
            symbolTable.insert(id, currentLevel, offset, "parameter");
            paramIndex++;
        }
    }

    node.block->accept(*this);

    // Return
    emit("RTPR", currentLevel, numParams);

    symbolTable.exitScope();
    currentLevel--;
    emitLabel(endLabel);
}

void CodeGenerator::visit(FuncDecl& node) {
    std::string label = newLabel();
    std::string endLabel = newLabel();
    
    // Register function in current scope
    int numParams = 0;
    for (const auto& param : node.params) {
        numParams += param->idList.size();
    }
    symbolTable.insert(node.funcName, currentLevel, 0, "function", label, numParams);

    emit("DSVS", endLabel);
    emitLabel(label);
    emit("ENPR", currentLevel + 1);

    currentLevel++;
    symbolTable.enterScope();

    // Register return variable
    // Offset is -4 - n - 1 = -5 - n
    int returnOffset = -5 - numParams;
    symbolTable.insert(node.funcName, currentLevel, returnOffset, "variable");

    // Register parameters
    int paramIndex = 0;
    for (const auto& param : node.params) {
        for (const auto& id : param->idList) {
            int offset = -5 - (numParams - 1 - paramIndex);
            symbolTable.insert(id, currentLevel, offset, "parameter");
            paramIndex++;
        }
    }

    node.block->accept(*this);

    // Return
    emit("RTPR", currentLevel, numParams);

    symbolTable.exitScope();
    currentLevel--;
    emitLabel(endLabel);
}

void CodeGenerator::visit(ParamDecl& node) {
    // Handled in ProcDecl/FuncDecl
}

void CodeGenerator::visit(TypeNode& node) {
    // Nothing to generate
}

void CodeGenerator::visit(CompoundStmt& node) {
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void CodeGenerator::visit(AssignStmt& node) {
    node.expression->accept(*this);
    SymbolInfo* info = symbolTable.lookup(node.varName);
    if (info) {
        emit("ARMZ", info->level, info->offset);
    } else {
        std::cerr << "Error: Variable " << node.varName << " not declared." << std::endl;
    }
}

void CodeGenerator::visit(IfStmt& node) {
    std::string elseLabel = newLabel();
    std::string endLabel = newLabel();

    node.condition->accept(*this);
    emit("DSVF", elseLabel);

    node.thenStmt->accept(*this);
    emit("DSVS", endLabel);

    emitLabel(elseLabel);
    if (node.elseStmt) {
        node.elseStmt->accept(*this);
    }
    emitLabel(endLabel);
}

void CodeGenerator::visit(WhileStmt& node) {
    std::string startLabel = newLabel();
    std::string endLabel = newLabel();

    emitLabel(startLabel);
    node.condition->accept(*this);
    emit("DSVF", endLabel);

    node.body->accept(*this);
    emit("DSVS", startLabel);

    emitLabel(endLabel);
}

void CodeGenerator::visit(ProcCallStmt& node) {
    SymbolInfo* info = symbolTable.lookup(node.procName);
    if (info && info->type == "procedure") {
        for (const auto& arg : node.arguments) {
            arg->accept(*this);
        }
        emit("CHPR", info->label, info->level);

    } else {
        std::cerr << "Error: Procedure " << node.procName << " not declared." << std::endl;
    }
}

void CodeGenerator::visit(ReadStmt& node) {
    for (const auto& var : node.variables) {
        emit("LEIT");
        SymbolInfo* info = symbolTable.lookup(var->varName);
        if (info) {
            emit("ARMZ", info->level, info->offset);
        } else {
            std::cerr << "Error: Variable " << var->varName << " not declared." << std::endl;
        }
    }
}

void CodeGenerator::visit(WriteStmt& node) {
    for (const auto& expr : node.expressions) {
        expr->accept(*this);
        emit("IMPR");
    }
}

void CodeGenerator::visit(BinaryExpr& node) {
    node.left->accept(*this);
    node.right->accept(*this);

    if (node.op == "+") emit("SOMA");
    else if (node.op == "-") emit("SUBT");
    else if (node.op == "*") emit("MULT");
    else if (node.op == "/" || node.op == "div") emit("DIVI"); // Integer division
    else if (node.op == "and") emit("CONJ");
    else if (node.op == "or") emit("DISJ");
    else if (node.op == "<") emit("CMME");
    else if (node.op == ">") emit("CMMA");
    else if (node.op == "=") emit("CMIG"); // Equality
    else if (node.op == "<>") emit("CMDG"); // Inequality
    else if (node.op == "<=") emit("CMEG");
    else if (node.op == ">=") emit("CMAG");
}

void CodeGenerator::visit(UnaryExpr& node) {
    node.right->accept(*this);
    if (node.op == "-") emit("INVR");
    else if (node.op == "not") emit("NEGA");
}

void CodeGenerator::visit(NumberExpr& node) {
    emit("CRCT", node.value);
}

void CodeGenerator::visit(BoolExpr& node) {
    emit("CRCT", node.value ? 1 : 0);
}

void CodeGenerator::visit(VarExpr& node) {
    SymbolInfo* info = symbolTable.lookup(node.varName);
    if (info) {
        emit("CRVL", info->level, info->offset);
    } else {
        std::cerr << "Error: Variable " << node.varName << " not declared." << std::endl;
    }
}

void CodeGenerator::visit(CallExpr& node) {
    SymbolInfo* info = symbolTable.lookup(node.funcName);
    if (info && info->type == "function") {
        emit("AMEM", 1); // Space for return value
        for (const auto& arg : node.arguments) {
            arg->accept(*this);
        }
        emit("CHPR", info->label, info->level); 
       
    } else {
        std::cerr << "Error: Function " << node.funcName << " not declared." << std::endl;
    }
}
