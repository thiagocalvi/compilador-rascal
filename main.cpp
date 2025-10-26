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