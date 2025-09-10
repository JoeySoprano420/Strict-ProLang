# strictc/src/lexer.cpp
#include "lexer.hpp"
#include <cctype>

Token Lexer::next() {
    while (isspace(peek())) advance();
    if (eof()) return {TOK_EOF, ""};

    char c = advance();
    if (isalpha(c)) {
        std::string ident(1, c);
        while (isalnum(peek())) ident += advance();
        if (ident == "Let") return {TOK_LET, ident};
        if (ident == "If") return {TOK_IF, ident};
        if (ident == "Else") return {TOK_ELSE, ident};
        if (ident == "End") return {TOK_END, ident};
        if (ident == "Func") return {TOK_FUNC, ident};
        if (ident == "Class") return {TOK_CLASS, ident};
        if (ident == "Return") return {TOK_RETURN, ident};
        if (ident == "New") return {TOK_NEW, ident};
        if (ident == "Match") return {TOK_MATCH, ident};
        if (ident == "Case") return {TOK_CASE, ident};
        if (ident == "Print") return {TOK_PRINT, ident};
        if (ident == "Input") return {TOK_INPUT, ident};
        if (ident == "Module") return {TOK_MODULE, ident};
        if (ident == "Import") return {TOK_IMPORT, ident};
        if (ident == "Macro") return {TOK_MACRO, ident};
        if (ident == "Template") return {TOK_TEMPLATE, ident};
        return {TOK_IDENTIFIER, ident};
    }
    if (isdigit(c)) {
        std::string num(1, c);
        while (isdigit(peek())) num += advance();
        return {TOK_NUMBER, num, std::stoi(num)};
    }
    if (c == '"') {
        std::string str;
        while (peek() != '"' && !eof()) str += advance();
        advance();
        return {TOK_STRING, str};
    }
    if (c == '.' && peek() == '.') { advance(); return {TOK_DOTDOT, ".."}; }
    return {TOK_OP, std::string(1, c)};
}
