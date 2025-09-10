# strictc/include/parser.hpp
#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <vector>

class Parser {
    Lexer& lexer;
    Token cur;
public:
    Parser(Lexer& l):lexer(l){ cur=lexer.next(); }
    std::vector<Stmt*> parse();
private:
    void consume(TokenType t, const std::string& msg);
    Expr* parseExpr();
    Expr* parsePrimary();
    Expr* parseBinaryRHS(int prec, Expr* lhs);
    Stmt* parseStmt();
    Stmt* parseLet();
    Stmt* parsePrint();
    Stmt* parseIf();
};
