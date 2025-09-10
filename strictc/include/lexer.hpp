#pragma once
#include <string>
#include <vector>

enum TokenType {
    TOK_EOF, TOK_ERROR,
    TOK_LET, TOK_IF, TOK_ELSE, TOK_END,
    TOK_FUNC, TOK_RETURN,
    TOK_CLASS, TOK_NEW,
    TOK_MATCH, TOK_CASE,
    TOK_PRINT, TOK_INPUT,
    TOK_MODULE, TOK_IMPORT,
    TOK_MACRO, TOK_TEMPLATE,
    TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING,
    TOK_OP, TOK_DOTDOT,
    TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    TOK_LBRACK, TOK_RBRACK,
    TOK_COMMA, TOK_COLON,
    TOK_EQUAL
};

struct Token {
    TokenType type;
    std::string text;
    int value;
};

class Lexer {
    std::string src;
    size_t pos = 0;
    int line = 1, col = 1;

public:
    Lexer(const std::string& s) : src(s) {}
    Token next();

private:
    char peek() const { return pos < src.size() ? src[pos] : '\0'; }
    char advance() {
        if (pos < src.size()) {
            char c = src[pos++];
            if (c == '\n') { line++; col = 1; }
            else col++;
            return c;
        }
        return '\0';
    }
    bool eof() const { return pos >= src.size(); }
    void skipWhitespace();
    void skipComment();
};
