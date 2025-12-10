#include "SemanticChecker.hpp"
#include "../ast/ASTNodes.hpp"

SemanticChecker::SemanticChecker() {}

int SemanticChecker::getErrorCount() const {
    return errorCount;
}

void SemanticChecker::logError(const std::string& msg) {
    std::cerr << "ERRO SEMÂNTICO: " << msg << std::endl;
    errorCount++;
}

void SemanticChecker::visit(ProgramNode& node) {
    symTable.insert(node.programName, std::make_shared<Symbol>(node.programName, SymbolCategory::PROGRAM));

    if (node.block) {
        node.block->accept(*this);
    }
}

void SemanticChecker::visit(BlockNode& node) {
    // Visit variable declarations
    for (auto& varDecl : node.varDeclarations) {
        varDecl->accept(*this);
    }

    // Visit subroutines
    for (auto& sub : node.subroutineDeclarations) {
        sub->accept(*this);
    }

    // Visit compound statement
    if (node.compoundStatement) {
        node.compoundStatement->accept(*this);
    }
}

void SemanticChecker::visit(VarDecl& node) {
    std::string typeName = node.type->typeName;
    for (const auto& id : node.idList) {
        if (symTable.existsInCurrentScope(id)) {
            logError("Variável '" + id + "' já declarada neste escopo.");
        } else {
            symTable.insert(id, std::make_shared<Symbol>(id, SymbolCategory::VARIABLE, typeName));
        }
    }
}

void SemanticChecker::visit(TypeNode& node) {
}

void SemanticChecker::visit(ProcDecl& node) {
    if (symTable.existsInCurrentScope(node.procName)) {
        logError("Procedimento '" + node.procName + "' já declarado neste escopo.");
    } else {
        auto sym = std::make_shared<Symbol>(node.procName, SymbolCategory::PROCEDURE);
        for (auto& param : node.params) {
             for (size_t i = 0; i < param->idList.size(); ++i) {
                 sym->paramTypes.push_back(param->type->typeName);
             }
        }
        symTable.insert(node.procName, sym);
    }

    symTable.enterScope();
    
    // Insert parameters into local scope
    for (auto& param : node.params) {
        param->accept(*this);
    }

    if (node.block) {
        node.block->accept(*this);
    }

    symTable.exitScope();
}

void SemanticChecker::visit(FuncDecl& node) {
    if (symTable.existsInCurrentScope(node.funcName)) {
        logError("Função '" + node.funcName + "' já declarada neste escopo.");
    } else {
        auto sym = std::make_shared<Symbol>(node.funcName, SymbolCategory::FUNCTION, node.returnType->typeName);
        for (auto& param : node.params) {
             for (size_t i = 0; i < param->idList.size(); ++i) {
                 sym->paramTypes.push_back(param->type->typeName);
             }
        }
        symTable.insert(node.funcName, sym);
    }

    symTable.enterScope();

    // Insert parameters into local scope
    for (auto& param : node.params) {
        param->accept(*this);
    }
    
    symTable.insert(node.funcName, std::make_shared<Symbol>(node.funcName, SymbolCategory::VARIABLE, node.returnType->typeName));

    if (node.block) {
        node.block->accept(*this);
    }

    symTable.exitScope();
}

void SemanticChecker::visit(ParamDecl& node) {
    std::string typeName = node.type->typeName;
    for (const auto& id : node.idList) {
        if (symTable.existsInCurrentScope(id)) {
            logError("Parâmetro '" + id + "' duplicado.");
        } else {
            symTable.insert(id, std::make_shared<Symbol>(id, SymbolCategory::PARAMETER, typeName));
        }
    }
}

void SemanticChecker::visit(CompoundStmt& node) {
    for (auto& stmt : node.statements) {
        stmt->accept(*this);
    }
}

void SemanticChecker::visit(AssignStmt& node) {
    auto sym = symTable.lookup(node.varName);
    if (!sym) {
        logError("Variável '" + node.varName + "' não declarada.");
        return;
    }
    
    if (sym->category != SymbolCategory::VARIABLE && sym->category != SymbolCategory::PARAMETER && sym->category != SymbolCategory::FUNCTION) {
         logError("Atribuição inválida para '" + node.varName + "'.");
    }

    node.expression->accept(*this);
    std::string exprType = lastType;

    if (sym->type != exprType) {
        logError("Tipos incompatíveis na atribuição para '" + node.varName + "'. Esperado: " + sym->type + ", Encontrado: " + exprType);
    }
}

void SemanticChecker::visit(IfStmt& node) {
    node.condition->accept(*this);
    if (lastType != "boolean") {
        logError("A condição do 'if' deve ser do tipo boolean.");
    }

    if (node.thenStmt) node.thenStmt->accept(*this);
    if (node.elseStmt) node.elseStmt->accept(*this);
}

void SemanticChecker::visit(WhileStmt& node) {
    node.condition->accept(*this);
    if (lastType != "boolean") {
        logError("A condição do 'while' deve ser do tipo boolean.");
    }

    if (node.body) node.body->accept(*this);
}

void SemanticChecker::visit(ProcCallStmt& node) {
    auto sym = symTable.lookup(node.procName);
    if (!sym) {
        logError("Procedimento '" + node.procName + "' não declarado.");
        return;
    }

    if (sym->category != SymbolCategory::PROCEDURE) {
        logError("'" + node.procName + "' não é um procedimento.");
        return;
    }

    if (node.arguments.size() != sym->paramTypes.size()) {
        logError("Número incorreto de argumentos para '" + node.procName + "'.");
        return;
    }

    for (size_t i = 0; i < node.arguments.size(); ++i) {
        node.arguments[i]->accept(*this);
        if (lastType != sym->paramTypes[i]) {
            logError("Tipo de argumento incompatível na chamada de '" + node.procName + "'.");
        }
    }
}

void SemanticChecker::visit(ReadStmt& node) {
    for (auto& var : node.variables) {
        var->accept(*this);
    }
}

void SemanticChecker::visit(WriteStmt& node) {
    for (auto& expr : node.expressions) {
        expr->accept(*this);
    }
}

void SemanticChecker::visit(BinaryExpr& node) {
    node.left->accept(*this);
    std::string leftType = lastType;
    
    node.right->accept(*this);
    std::string rightType = lastType;

    if (node.op == "+" || node.op == "-" || node.op == "*" || node.op == "div") {
        if (leftType == "integer" && rightType == "integer") {
            lastType = "integer";
        } else {
            logError("Operação aritmética '" + node.op + "' requer operandos inteiros.");
            lastType = "integer";
        }
    } else if (node.op == "and" || node.op == "or") {
        if (leftType == "boolean" && rightType == "boolean") {
            lastType = "boolean";
        } else {
            logError("Operação lógica '" + node.op + "' requer operandos booleanos.");
            lastType = "boolean";
        }
    } else if (node.op == "=" || node.op == "<>" || node.op == "<" || node.op == "<=" || node.op == ">" || node.op == ">=") {
        if (leftType == rightType) {
            lastType = "boolean";
        } else {
            logError("Operação relacional '" + node.op + "' requer operandos do mesmo tipo.");
            lastType = "boolean";
        }
    }
}

void SemanticChecker::visit(UnaryExpr& node) {
    node.right->accept(*this);
    std::string type = lastType;

    if (node.op == "not") {
        if (type == "boolean") {
            lastType = "boolean";
        } else {
            logError("Operador 'not' requer operando booleano.");
            lastType = "boolean";
        }
    } else if (node.op == "-") {
        if (type == "integer") {
            lastType = "integer";
        } else {
            logError("Operador unário '-' requer operando inteiro.");
            lastType = "integer";
        }
    }
}

void SemanticChecker::visit(NumberExpr& node) {
    lastType = "integer";
}

void SemanticChecker::visit(BoolExpr& node) {
    lastType = "boolean";
}

void SemanticChecker::visit(VarExpr& node) {
    auto sym = symTable.lookup(node.varName);
    if (!sym) {
        logError("Variável '" + node.varName + "' não declarada.");
        lastType = "integer"; // Fallback
    } else {
        lastType = sym->type;
    }
}

void SemanticChecker::visit(CallExpr& node) {
    auto sym = symTable.lookup(node.funcName);
    if (!sym) {
        logError("Função '" + node.funcName + "' não declarada.");
        lastType = "integer"; // Fallback
        return;
    }

    if (sym->category != SymbolCategory::FUNCTION) {
        logError("'" + node.funcName + "' não é uma função.");
        lastType = "integer"; // Fallback
        return;
    }

    if (node.arguments.size() != sym->paramTypes.size()) {
        logError("Número incorreto de argumentos para '" + node.funcName + "'.");
    } else {
        for (size_t i = 0; i < node.arguments.size(); ++i) {
            node.arguments[i]->accept(*this);
            if (lastType != sym->paramTypes[i]) {
                logError("Tipo de argumento incompatível na chamada de '" + node.funcName + "'.");
            }
        }
    }
    
    lastType = sym->type;
}
