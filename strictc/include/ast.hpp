#pragma once
#include <string>
#include <vector>

struct Expr { virtual ~Expr() {} };
struct Stmt { virtual ~Stmt() {} };

struct NumberExpr : Expr { int value; NumberExpr(int v):value(v){} };
struct StringExpr : Expr { std::string text; StringExpr(const std::string& t):text(t){} };
struct VarExpr : Expr { std::string name; VarExpr(const std::string& n):name(n){} };
struct BinaryExpr : Expr {
    std::string op; Expr* lhs; Expr* rhs;
    BinaryExpr(const std::string& o, Expr* l, Expr* r):op(o),lhs(l),rhs(r){}
};
struct CallExpr : Expr {
    std::string callee; std::vector<Expr*> args;
    CallExpr(const std::string& c,const std::vector<Expr*>& a):callee(c),args(a){}
};

struct LetStmt : Stmt { std::string name; Expr* expr; };
struct PrintStmt : Stmt { Expr* expr; };
struct ReturnStmt : Stmt { Expr* expr; };
struct BlockStmt : Stmt { std::vector<Stmt*> body; };
struct IfStmt : Stmt { Expr* cond; std::vector<Stmt*> thenBody; std::vector<Stmt*> elseBody; };
struct FuncStmt : Stmt { std::string name; std::vector<std::string> params; std::vector<Stmt*> body; };
struct ClassStmt : Stmt { std::string name; std::vector<Stmt*> body; };
struct CaseStmt { Expr* pattern; std::vector<Stmt*> body; };
struct MatchStmt : Stmt { Expr* target; std::vector<CaseStmt> cases; };
