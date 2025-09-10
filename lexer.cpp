#include "lexer.hpp"
#include <cctype>
#include <stdexcept>

void Lexer::skipWhitespace() {
    while (isspace(peek())) advance();
}

void Lexer::skipComment() {
    // Strict uses "--" for comments
    if (peek() == '-' && pos + 1 < src.size() && src[pos+1] == '-') {
        while (peek() != '\n' && !eof()) advance();
    }
}

Token Lexer::next() {
    skipWhitespace();
    skipComment();
    skipWhitespace();

    if (eof()) return {TOK_EOF, "", 0};

    char c = advance();

    // ===== Identifiers / Keywords =====
    if (isalpha(c) || c == '_') {
        std::string ident(1, c);
        while (isalnum(peek()) || peek() == '_') ident += advance();

        if (ident == "Let") return {TOK_LET, ident};
        if (ident == "If") return {TOK_IF, ident};
        if (ident == "Else") return {TOK_ELSE, ident};
        if (ident == "End") return {TOK_END, ident};
        if (ident == "Func") return {TOK_FUNC, ident};
        if (ident == "Return") return {TOK_RETURN, ident};
        if (ident == "Class") return {TOK_CLASS, ident};
        if (ident == "New") return {TOK_NEW, ident};
        if (ident == "Match") return {TOK_MATCH, ident};
        if (ident == "Case") return {TOK_CASE, ident};
        if (ident == "Print") return {TOK_PRINT, ident};
        if (ident == "Input") return {TOK_INPUT, ident};
        if (ident == "Module") return {TOK_MODULE, ident};
        if (ident == "Import") return {TOK_IMPORT, ident};
        if (ident == "Macro") return {TOK_MACRO, ident};
        if (ident == "Template") return {TOK_TEMPLATE, ident};

        return {TOK_IDENTIFIER, ident, 0};
    }

    // ===== Numbers =====
    if (isdigit(c)) {
        std::string num(1, c);
        while (isdigit(peek())) num += advance();
        return {TOK_NUMBER, num, std::stoi(num)};
    }

    // ===== Strings =====
    if (c == '"') {
        std::string str;
        while (peek() != '"' && !eof()) str += advance();
        if (peek() == '"') advance();
        return {TOK_STRING, str, 0};
    }

    // ===== Multi-char Operators =====
    if (c == '.' && peek() == '.') {
        advance();
        return {TOK_DOTDOT, "..", 0};
    }
    if (c == '=' ) {
        return {TOK_EQUAL, "=", 0};
    }

    // ===== Single-char Tokens =====
    switch(c) {
        case '(': return {TOK_LPAREN, "(", 0};
        case ')': return {TOK_RPAREN, ")", 0};
        case '{': return {TOK_LBRACE, "{", 0};
        case '}': return {TOK_RBRACE, "}", 0};
        case '[': return {TOK_LBRACK, "[", 0};
        case ']': return {TOK_RBRACK, "]", 0};
        case ',': return {TOK_COMMA, ",", 0};
        case ':': return {TOK_COLON, ":", 0};
        default:  return {TOK_OP, std::string(1,c), 0};
    }
}
