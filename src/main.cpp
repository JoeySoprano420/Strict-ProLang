#include "lexer.hpp"
#include "parser.hpp"
#include "dgm_emitter.hpp"
#include "dgm_translator.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: strictc <source.strict>\n";
        return 1;
    }

    std::ifstream f(argv[1]);
    if (!f) {
        std::cerr << "File not found: " << argv[1] << "\n";
        return 1;
    }
    std::string src((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    try {
        // Lex + Parse
        Lexer lex(src);
        Parser parser(lex);
        auto prog = parser.parseProgram();

        // Strict → DGM
        DGMEmitter emitter;
        emitter.gen(prog);
        emitter.write("out.dgm");

        // DGM → LLVM IR
        DGMTranslator translator;
        translator.translate("out.dgm", "out.ll");

        std::cout << "Compilation successful.\n";
        std::cout << "Generated out.dgm and out.ll\n";
        std::cout << "Build native binary with:\n";
        std::cout << "  clang out.ll src/runtime.c -o prog\n";
        std::cout << "Run with:\n";
        std::cout << "  ./prog\n";
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
