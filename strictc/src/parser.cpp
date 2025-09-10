# strictc/src/parser.cpp
#include "parser.hpp"
#include <stdexcept>

static int precedence(const std::string& op) {
    if (op=="*"||op=="/") return 20;
    if (op=="+"||op=="-") return 10;
    return -1;
}

void Parser::consume(TokenType t,const std::string& msg){
    if(cur.type!=t) throw std::runtime_error("Expected "+msg);
    cur=lexer.next();
}

Expr* Parser::parsePrimary(){
    if(cur.type==TOK_NUMBER){ int v=cur.value; cur=lexer.next(); return new NumberExpr(v);}
    if(cur.type==TOK_STRING){ std::string s=cur.text; cur=lexer.next(); return new StringExpr(s);}
    if(cur.type==TOK_IDENTIFIER){ std::string n=cur.text; cur=lexer.next(); return new VarExpr(n);}
    throw std::runtime_error("Unexpected token in expression");
}

Expr* Parser::parseBinaryRHS(int exprPrec,Expr* lhs){
    while(true){
        if(cur.type!=TOK_OP) return lhs;
        std::string op=cur.text;
        int tokPrec=precedence(op);
        if(tokPrec<exprPrec) return lhs;
        cur=lexer.next();
        Expr* rhs=parsePrimary();
        int nextPrec=precedence(cur.text);
        if(tokPrec<nextPrec) rhs=parseBinaryRHS(tokPrec+1,rhs);
        lhs=new BinaryExpr(op,lhs,rhs);
    }
}

Expr* Parser::parseExpr(){
    Expr* lhs=parsePrimary();
    return parseBinaryRHS(0,lhs);
}

Stmt* Parser::parseLet(){
    consume(TOK_LET,"Let");
    std::string name=cur.text; consume(TOK_IDENTIFIER,"identifier");
    consume(TOK_OP,"=");
    Expr* val=parseExpr();
    auto s=new LetStmt(); s->name=name; s->expr=val;
    return s;
}

Stmt* Parser::parsePrint(){
    consume(TOK_PRINT,"Print");
    Expr* e=parseExpr();
    auto s=new PrintStmt(); s->expr=e; return s;
}

Stmt* Parser::parseIf(){
    consume(TOK_IF,"If");
    Expr* cond=parseExpr();
    auto s=new IfStmt(); s->cond=cond;
    while(cur.type!=TOK_END && cur.type!=TOK_ELSE){
        s->thenBody.push_back(parseStmt());
    }
    if(cur.type==TOK_ELSE){
        cur=lexer.next();
        while(cur.type!=TOK_END){
            s->elseBody.push_back(parseStmt());
        }
    }
    consume(TOK_END,"End");
    return s;
}

Stmt* Parser::parseStmt(){
    if(cur.type==TOK_LET) return parseLet();
    if(cur.type==TOK_PRINT) return parsePrint();
    if(cur.type==TOK_IF) return parseIf();
    throw std::runtime_error("Unknown statement");
}

std::vector<Stmt*> Parser::parse(){
    std::vector<Stmt*> prog;
    while(cur.type!=TOK_EOF){
        prog.push_back(parseStmt());
    }
    return prog;
}
