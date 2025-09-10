#include "parser.hpp"
#include <iostream>

// ===== Utility =====
void Parser::consume(TokenType t, const std::string& msg) {
    if (cur.type != t) {
        throw std::runtime_error("Parse error: expected " + msg + ", got " + cur.text);
    }
    cur = lexer.next();
}

bool Parser::match(TokenType t) {
    if (cur.type == t) {
        cur = lexer.next();
        return true;
    }
    return false;
}

// ===== Operator Precedence =====
int Parser::precedence(const std::string& op) {
    if (op == "*" || op == "/") return 20;
    if (op == "+" || op == "-") return 10;
    if (op == ">" || op == "<" || op == "=") return 5;
    return -1;
}

// ===== Expressions =====
Expr* Parser::parsePrimary() {
    if (cur.type == TOK_NUMBER) {
        int v = cur.value;
        advance();
        return new NumberExpr(v);
    }
    if (cur.type == TOK_STRING) {
        std::string s = cur.text;
        advance();
        return new StringExpr(s);
    }
    if (cur.type == TOK_IDENTIFIER) {
        std::string n = cur.text;
        advance();

        // function call?
        if (check(TOK_LPAREN)) {
            advance();
            std::vector<Expr*> args;
            if (!check(TOK_RPAREN)) {
                do {
                    args.push_back(parseExpr());
                } while (match(TOK_COMMA));
            }
            consume(TOK_RPAREN, ")");
            return new CallExpr(n, args);
        }
        return new VarExpr(n);
    }
    if (cur.type == TOK_LPAREN) {
        advance();
        Expr* e = parseExpr();
        consume(TOK_RPAREN, ")");
        return e;
    }
    throw std::runtime_error("Unexpected token in expression: " + cur.text);
}

Expr* Parser::parseBinaryRHS(int exprPrec, Expr* lhs) {
    while (true) {
        if (cur.type != TOK_OP) return lhs;
        std::string op = cur.text;
        int tokPrec = precedence(op);
        if (tokPrec < exprPrec) return lhs;

        advance();
        Expr* rhs = parsePrimary();
        int nextPrec = (cur.type == TOK_OP) ? precedence(cur.text) : -1;
        if (tokPrec < nextPrec) {
            rhs = parseBinaryRHS(tokPrec + 1, rhs);
        }
        lhs = new BinaryExpr(op, lhs, rhs);
    }
}

Expr* Parser::parseExpr() {
    Expr* lhs = parsePrimary();
    return parseBinaryRHS(0, lhs);
}

// ===== Statements =====
Stmt* Parser::parseLet() {
    consume(TOK_LET, "Let");
    std::string name = cur.text;
    consume(TOK_IDENTIFIER, "identifier");
    consume(TOK_EQUAL, "=");
    Expr* val = parseExpr();
    auto s = new LetStmt();
    s->name = name;
    s->expr = val;
    return s;
}

Stmt* Parser::parsePrint() {
    consume(TOK_PRINT, "Print");
    Expr* e = parseExpr();
    auto s = new PrintStmt();
    s->expr = e;
    return s;
}

Stmt* Parser::parseIf() {
    consume(TOK_IF, "If");
    Expr* cond = parseExpr();
    auto s = new IfStmt();
    s->cond = cond;

    while (!check(TOK_END) && !check(TOK_ELSE) && cur.type != TOK_EOF) {
        s->thenBody.push_back(parseStmt());
    }

    if (match(TOK_ELSE)) {
        while (!check(TOK_END) && cur.type != TOK_EOF) {
            s->elseBody.push_back(parseStmt());
        }
    }

    consume(TOK_END, "End");
    return s;
}

Stmt* Parser::parseReturn() {
    consume(TOK_RETURN, "Return");
    Expr* e = parseExpr();
    auto s = new ReturnStmt();
    s->expr = e;
    return s;
}

Stmt* Parser::parseFunc() {
    consume(TOK_FUNC, "Func");
    std::string name = cur.text;
    consume(TOK_IDENTIFIER, "function name");

    std::vector<std::string> params;
    if (match(TOK_LPAREN)) {
        if (!check(TOK_RPAREN)) {
            do {
                std::string param = cur.text;
                consume(TOK_IDENTIFIER, "parameter");
                params.push_back(param);
            } while (match(TOK_COMMA));
        }
        consume(TOK_RPAREN, ")");
    }

    auto f = new FuncStmt();
    f->name = name;
    f->params = params;

    while (!check(TOK_END) && cur.type != TOK_EOF) {
        f->body.push_back(parseStmt());
    }
    consume(TOK_END, "End");
    return f;
}

Stmt* Parser::parseClass() {
    consume(TOK_CLASS, "Class");
    std::string name = cur.text;
    consume(TOK_IDENTIFIER, "class name");

    auto c = new ClassStmt();
    c->name = name;

    while (!check(TOK_END) && cur.type != TOK_EOF) {
        c->body.push_back(parseStmt());
    }
    consume(TOK_END, "End");
    return c;
}

Stmt* Parser::parseMatch() {
    consume(TOK_MATCH, "Match");
    Expr* target = parseExpr();
    auto m = new MatchStmt();
    m->target = target;

    while (!check(TOK_END) && cur.type != TOK_EOF) {
        consume(TOK_CASE, "Case");
        Expr* pat = parseExpr();
        CaseStmt caseNode;
        caseNode.pattern = pat;

        while (!check(TOK_END) && !check(TOK_CASE) && cur.type != TOK_EOF) {
            caseNode.body.push_back(parseStmt());
        }
        consume(TOK_END, "End");
        m->cases.push_back(caseNode);
    }

    consume(TOK_END, "End");
    return m;
}

Stmt* Parser::parseStmt() {
    if (check(TOK_LET)) return parseLet();
    if (check(TOK_PRINT)) return parsePrint();
    if (check(TOK_IF)) return parseIf();
    if (check(TOK_RETURN)) return parseReturn();
    if (check(TOK_FUNC)) return parseFunc();
    if (check(TOK_CLASS)) return parseClass();
    if (check(TOK_MATCH)) return parseMatch();
    throw std::runtime_error("Unexpected token in statement: " + cur.text);
}

// ===== Program =====
std::vector<Stmt*> Parser::parseProgram() {
    std::vector<Stmt*> prog;
    while (cur.type != TOK_EOF) {
        prog.push_back(parseStmt());
    }
    return prog;
}
