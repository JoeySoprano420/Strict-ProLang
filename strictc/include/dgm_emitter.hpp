# strictc/include/dgm_emitter.hpp
#pragma once
#include "ast.hpp"
#include <vector>
#include <string>

class DGMEmitter {
    std::vector<std::string> code;
public:
    void emit(const std::string& s){ code.push_back(s); }
    void gen(std::vector<Stmt*>& prog);
    void write(const std::string& path);
private:
    void genStmt(Stmt* s);
    std::string genExpr(Expr* e);
};
