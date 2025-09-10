# strictc/src/driver.cpp
#include "lexer.hpp"
#include "parser.hpp"
#include "dgm_emitter.hpp"
#include "dgm_translator.hpp"
#include <fstream>
#include <iostream>

int main(int argc,char**argv){
    if(argc<2){ std::cerr<<"Usage: strictc <file.strict>\n"; return 1;}
    std::ifstream f(argv[1]);
    std::string src((std::istreambuf_iterator<char>(f)),{});
    Lexer lex(src);
    Parser parser(lex);
    auto prog=parser.parse();

    DGMEmitter emitter;
    emitter.gen(prog);
    emitter.write("out.dgm");

    DGMTranslator trans;
    trans.translate("out.dgm","out.ll");
    std::cout<<"Generated out.dgm and out.ll\n";
}
