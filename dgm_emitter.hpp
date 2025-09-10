#pragma once
#include "ast.hpp"
#include <string>
#include <vector>

class DGMEmitter {
    std::vector<std::string> code;
    int tmpCount = 0;

public:
    void emit(const std::string& s) { code.push_back(s); }
    void gen(std::vector<Stmt*>& prog);
    void write(const std::string& path);

private:
    // expression lowering
    std::string genExpr(Expr* e);

    // statement lowering
    void genStmt(Stmt* s);

    // helpers
    std::string newTmp() { return "%t" + std::to_string(++tmpCount); }
};
