#pragma once

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <iostream>
#include <memory>

enum class SymbolCategory {
    PROGRAM,
    VARIABLE,
    PROCEDURE,
    FUNCTION,
    PARAMETER
};

struct Symbol {
    std::string name;
    SymbolCategory category;
    std::string type;
    std::vector<std::string> paramTypes;

    Symbol(std::string n, SymbolCategory c, std::string t = "")
        : name(n), category(c), type(t) {}
};

class SymbolTable {
private:
    std::deque<std::map<std::string, std::shared_ptr<Symbol>>> scopes;

public:
    SymbolTable() {
        enterScope();
    }

    void enterScope() {
        scopes.push_back(std::map<std::string, std::shared_ptr<Symbol>>());
    }

    void exitScope() {
        if (!scopes.empty()) {
            scopes.pop_back();
        }
    }

    bool insert(const std::string& name, std::shared_ptr<Symbol> symbol) {
        if (scopes.empty()) return false;

        auto& currentScope = scopes.back();
        if (currentScope.find(name) != currentScope.end()) {
            return false;
        }
        currentScope[name] = symbol;
        return true;
    }

    std::shared_ptr<Symbol> lookup(const std::string& name) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
        return nullptr;
    }

    bool existsInCurrentScope(const std::string& name) {
        if (scopes.empty()) return false;
        auto& currentScope = scopes.back();
        return currentScope.find(name) != currentScope.end();
    }
};
