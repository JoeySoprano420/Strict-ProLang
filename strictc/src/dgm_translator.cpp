# strictc/src/dgm_translator.cpp
#include "dgm_translator.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void DGMTranslator::translate(const std::string& inFile,const std::string& outFile){
    std::ifstream in(inFile);
    std::ofstream out(outFile);
    std::string line;
    while(std::getline(in,line)){
        if(line.find("17 add")!=std::string::npos){
            out<<"  %tmp = add i32 %a, %b\n";
        } else if(line.find("18 sub")!=std::string::npos){
            out<<"  %tmp = sub i32 %a, %b\n";
        } else if(line.find("A6 echo")!=std::string::npos){
            out<<"  call void @puts(i8* getelementptr ([20 x i8], [20 x i8]* @.str, i32 0, i32 0))\n";
        } else {
            out<<"; unhandled: "<<line<<"\n";
        }
    }
}
