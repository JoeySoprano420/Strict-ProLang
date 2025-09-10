#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <vector>
#include <stdexcept>
#include <string>

class Parser {
    Lexer& lexer;
    Token cur;

public:
    Parser(Lexer& l) : lexer(l) { cur = lexer.next(); }

    std::vector<Stmt*> parseProgram();

private:
    // utility
    void consume(TokenType t, const std::string& msg);
    bool match(TokenType t);
    bool check(TokenType t) const { return cur.type == t; }
    void advance() { cur = lexer.next(); }

    // expressions
    Expr* parseExpr();
    Expr* parsePrimary();
    Expr* parseBinaryRHS(int exprPrec, Expr* lhs);

    // statements
    Stmt* parseStmt();
    Stmt* parseLet();
    Stmt* parsePrint();
    Stmt* parseIf();
    Stmt* parseReturn();
    Stmt* parseFunc();
    Stmt* parseClass();
    Stmt* parseMatch();

    // precedence
    int precedence(const std::string& op);
};
