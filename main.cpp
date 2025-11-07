#include "./ast/ASTPrinter.hpp" 
#include "./ast/ASTNodes.hpp"

int main() {
    // while i < n do
    // begin
    //   f := f * i;
    //   i := i + 1
    // end;

    // i < n
    auto i_var1 = std::make_unique<VarExpr>("i");
    auto n_var = std::make_unique<VarExpr>("n");
    auto condition = std::make_unique<BinaryExpr>(std::move(i_var1), "<", std::move(n_var));
    
    // f := f * i
    auto f_var1 = std::make_unique<VarExpr>("f");
    auto i_var2 = std::make_unique<VarExpr>("i");
    auto rhs1 = std::make_unique<BinaryExpr>(std::move(f_var1), "*", std::move(i_var2));
    auto assign1 = std::make_unique<AssignStmt>("f", std::move(rhs1));

    // i := i + 1
    auto i_var3 = std::make_unique<VarExpr>("i");
    auto num1 = std::make_unique<NumberExpr>(1);
    auto rhs2 = std::make_unique<BinaryExpr>(std::move(i_var3), "+", std::move(num1));
    auto assign2 = std::make_unique<AssignStmt>("i", std::move(rhs2));

    std::vector<NodePtr<Stmt>> bodyStmts;
    bodyStmts.push_back(std::move(assign1));
    bodyStmts.push_back(std::move(assign2));

    auto body = std::make_unique<CompoundStmt>(std::move(bodyStmts));

    auto whileNode = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

    ASTPrinter printer;
    whileNode->accept(printer);

    return 0;
}


    // auto varDecl1 = std::make_unique<VarDecl>(
    //     std::vector<std::string>{"i", "n"}, 
    //     std::make_unique<TypeNode>("integer")
    // );

    // auto varDecl2 = std::make_unique<VarDecl>(
    //     std::vector<std::string>{"f"}, 
    //     std::make_unique<TypeNode>("integer")
    // );

    // std::vector<NodePtr<VarDecl>> varDecls;
    // varDecls.push_back(std::move(varDecl1));
    // varDecls.push_back(std::move(varDecl2));

    // std::vector<NodePtr<ASTNode>> subroutines; // Vazio

    // auto assign_n = std::make_unique<AssignStmt>("n", std::make_unique<NumberExpr>(5));
    // auto assign_i = std::make_unique<AssignStmt>("i", std::make_unique<NumberExpr>(1));
    // auto assign_f = std::make_unique<AssignStmt>("f", std::make_unique<NumberExpr>(1));


    // auto i_var1 = std::make_unique<VarExpr>("i");
    // auto n_var = std::make_unique<VarExpr>("n");
    // auto condition = std::make_unique<BinaryExpr>(std::move(i_var1), "<", std::move(n_var));

    // auto f_var1 = std::make_unique<VarExpr>("f");
    // auto i_var2 = std::make_unique<VarExpr>("i");
    // auto rhs1 = std::make_unique<BinaryExpr>(std::move(f_var1), "*", std::move(i_var2));
    // auto assign_loop1 = std::make_unique<AssignStmt>("f", std::move(rhs1));

    // auto i_var3 = std::make_unique<VarExpr>("i");
    // auto num1 = std::make_unique<NumberExpr>(1);
    // auto rhs2 = std::make_unique<BinaryExpr>(std::move(i_var3), "+", std::move(num1));
    // auto assign_loop2 = std::make_unique<AssignStmt>("i", std::move(rhs2));

    // std::vector<NodePtr<Stmt>> whileBodyStmts;
    // whileBodyStmts.push_back(std::move(assign_loop1));
    // whileBodyStmts.push_back(std::move(assign_loop2));
    
    // auto whileBody = std::make_unique<CompoundStmt>(std::move(whileBodyStmts));

    // auto whileNode = std::make_unique<WhileStmt>(std::move(condition), std::move(whileBody));

    // std::vector<NodePtr<Expr>> writeExprs;
    // writeExprs.push_back(std::make_unique<VarExpr>("f"));
    // auto writeStmt = std::make_unique<WriteStmt>(std::move(writeExprs));

    // std::vector<NodePtr<Stmt>> mainBodyStmts;
    // mainBodyStmts.push_back(std::move(assign_n));
    // mainBodyStmts.push_back(std::move(assign_i));
    // mainBodyStmts.push_back(std::move(assign_f));
    // mainBodyStmts.push_back(std::move(whileNode)); // Adiciona o loop
    // mainBodyStmts.push_back(std::move(writeStmt)); // Adiciona o write

    // auto mainCompoundStmt = std::make_unique<CompoundStmt>(std::move(mainBodyStmts));

    // auto mainBlock = std::make_unique<BlockNode>(
    //     std::move(varDecls),
    //     std::move(subroutines),
    //     std::move(mainCompoundStmt)
    // );

    // auto program = std::make_unique<ProgramNode>("Fatorial", std::move(mainBlock));

    // ASTPrinter printer;
    // program->accept(printer);