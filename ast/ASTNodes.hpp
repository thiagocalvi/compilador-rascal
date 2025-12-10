#pragma once

#include "ASTNode.hpp"
#include "ASTVisitor.hpp"
#include <map>

// type node
class TypeNode : public ASTNode {
public:
    std::string typeName; // interger, boolean
    TypeNode(const std::string& name) : typeName(name) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// number expression
class NumberExpr : public Expr {
public:
    int value;
    NumberExpr(int val) : value(val) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// boolean expression
class BoolExpr : public Expr {
public:
    bool value;
    BoolExpr(bool val) : value(val) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// variable expression
class VarExpr : public Expr {
public:
    std::string varName;
    VarExpr(const std::string& name) : varName(name) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// binary expression
class BinaryExpr : public Expr {
public:
    std::string op; // +, -, *, /, ...
    NodePtr<Expr> left;
    NodePtr<Expr> right;

    BinaryExpr(NodePtr<Expr> lhs, const std::string& op, NodePtr<Expr> rhs)
        : op(op), left(std::move(lhs)), right(std::move(rhs)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// unary expression
class UnaryExpr : public Expr {
public:
    std::string op; // -, not
    NodePtr<Expr> right;

    UnaryExpr(const std::string& op, NodePtr<Expr> rhs)
        : op(op), right(std::move(rhs)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// function call expression
class CallExpr : public Expr {
public:
    std::string funcName;
    std::vector<NodePtr<Expr>> arguments;

    CallExpr(const std::string& name, std::vector<NodePtr<Expr>> args)
        : funcName(name), arguments(std::move(args)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// compound statement
class CompoundStmt : public Stmt {
public:
    std::vector<NodePtr<Stmt>> statements;

    CompoundStmt(std::vector<NodePtr<Stmt>> stmts)
        : statements(std::move(stmts)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};


// assignment statement
class AssignStmt : public Stmt {
public:
    std::string varName;
    NodePtr<Expr> expression;

    AssignStmt(const std::string& name, NodePtr<Expr> expr)
        : varName(name), expression(std::move(expr)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// if statement
class IfStmt : public Stmt {
public:
    NodePtr<Expr> condition;
    NodePtr<Stmt> thenStmt;
    NodePtr<Stmt> elseStmt; // pode ser nulo

    IfStmt(NodePtr<Expr> cond, NodePtr<Stmt> thenBranch, NodePtr<Stmt> elseBranch)
        : condition(std::move(cond)), thenStmt(std::move(thenBranch)), elseStmt(std::move(elseBranch)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// while statement
class WhileStmt : public Stmt {
public:
    NodePtr<Expr> condition;
    NodePtr<Stmt> body;

    WhileStmt(NodePtr<Expr> cond, NodePtr<Stmt> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// procedure call statement
class ProcCallStmt : public Stmt {
public:
    std::string procName;
    std::vector<NodePtr<Expr>> arguments;

    ProcCallStmt(const std::string& name, std::vector<NodePtr<Expr>> args)
        : procName(name), arguments(std::move(args)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// read statement
class ReadStmt : public Stmt {
public:
    std::vector<NodePtr<VarExpr>> variables;

    ReadStmt(std::vector<NodePtr<VarExpr>> vars)
        : variables(std::move(vars)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// write statement
class WriteStmt : public Stmt {
public:
    std::vector<NodePtr<Expr>> expressions;

    WriteStmt(std::vector<NodePtr<Expr>> exprs)
        : expressions(std::move(exprs)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// declaration of a variable
class VarDecl : public Decl {
public:
    std::vector<std::string> idList;
    NodePtr<TypeNode> type;

    VarDecl(std::vector<std::string> ids, NodePtr<TypeNode> t)
        : idList(std::move(ids)), type(std::move(t)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// declaration of parameters
class ParamDecl : public Decl {
public:
    std::vector<std::string> idList;
    NodePtr<TypeNode> type;

    ParamDecl(std::vector<std::string> ids, NodePtr<TypeNode> t)
        : idList(std::move(ids)), type(std::move(t)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// block of declarations and body
class BlockNode : public ASTNode {
public:
    std::vector<NodePtr<VarDecl>> varDeclarations;
    std::vector<NodePtr<ASTNode>> subroutineDeclarations; // ProcDecl e FuncDecl
    NodePtr<CompoundStmt> compoundStatement;

    BlockNode(std::vector<NodePtr<VarDecl>> vars,
              std::vector<NodePtr<ASTNode>> subroutines,
              NodePtr<CompoundStmt> body)
        : varDeclarations(std::move(vars)),
          subroutineDeclarations(std::move(subroutines)),
          compoundStatement(std::move(body)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// declaration of a procedure
class ProcDecl : public Decl {
public:
    std::string procName;
    std::vector<NodePtr<ParamDecl>> params;
    NodePtr<BlockNode> block;

    ProcDecl(const std::string& name,
             std::vector<NodePtr<ParamDecl>> p,
             NodePtr<BlockNode> b)
        : procName(name), params(std::move(p)), block(std::move(b)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// declaration of a function
class FuncDecl : public Decl {
public:
    std::string funcName;
    std::vector<NodePtr<ParamDecl>> params;
    NodePtr<TypeNode> returnType;
    NodePtr<BlockNode> block;

    FuncDecl(const std::string& name,
             std::vector<NodePtr<ParamDecl>> p,
             NodePtr<TypeNode> r,
             NodePtr<BlockNode> b)
        : funcName(name), params(std::move(p)), returnType(std::move(r)), block(std::move(b)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};

// root of the AST
class ProgramNode : public ASTNode {
public:
    std::string programName;
    NodePtr<BlockNode> block;

    ProgramNode(const std::string& name, NodePtr<BlockNode> b)
        : programName(name), block(std::move(b)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }
};
