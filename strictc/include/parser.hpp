#pragma once
#include "lexer.hpp"
#include "ast.hpp"
#include <vector>
#include <stdexcept>

class Parser {
    Lexer& lexer;
    Token cur;

public:
    Parser(Lexer& l):lexer(l){cur=lexer.next();}
    std::vector<Stmt*> parseProgram();

private:
    void consume(TokenType t,const std::string& msg);
    bool match(TokenType t);
    bool check(TokenType t) const {return cur.type==t;}
    void advance(){cur=lexer.next();}

    Expr* parseExpr();
    Expr* parsePrimary();
    Expr* parseBinaryRHS(int exprPrec,Expr* lhs);

    Stmt* parseStmt();
    Stmt* parseLet();
    Stmt* parsePrint();
    Stmt* parseIf();
    Stmt* parseReturn();
    Stmt* parseFunc();
    Stmt* parseClass();
    Stmt* parseMatch();

    int precedence(const std::string& op);
};
