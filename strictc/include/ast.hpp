# strictc/include/ast.hpp
#pragma once
#include <string>
#include <vector>
#include <memory>

struct Expr { virtual ~Expr(){} };
struct NumberExpr : Expr { int value; NumberExpr(int v):value(v){} };
struct StringExpr : Expr { std::string text; StringExpr(std::string t):text(t){} };
struct VarExpr : Expr { std::string name; VarExpr(std::string n):name(n){} };
struct BinaryExpr : Expr { std::string op; Expr* lhs; Expr* rhs;
    BinaryExpr(std::string o, Expr* l, Expr* r):op(o),lhs(l),rhs(r){} };

struct Stmt { virtual ~Stmt(){} };
struct LetStmt : Stmt { std::string name; Expr* expr; };
struct PrintStmt : Stmt { Expr* expr; };
struct IfStmt : Stmt { Expr* cond; std::vector<Stmt*> thenBody, elseBody; };
struct ReturnStmt : Stmt { Expr* expr; };
