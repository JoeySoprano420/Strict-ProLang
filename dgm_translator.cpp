# strictc/src/dgm_translator.cpp
#include "dgm_translator.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void DGMTranslator::translate(const std::string& inFile,const std::string& outFile){
    std::ifstream in(inFile);
    std::ofstream out(outFile);
    std::string line;

    // LLVM IR preamble
    out << "declare i32 @puts(i8*)\n";
    out << "declare i32 @scanf(i8*, ...)\n";
    out << "@.str = private unnamed_addr constant [256 x i8] c\"%d\\00\", align 1\n";
    out << "define i32 @main() {\n";

    int tmpCount = 0;
    auto newTmp = [&](){ return "%t" + std::to_string(++tmpCount); };

    while(std::getline(in,line)){
        std::istringstream iss(line);
        std::string op; iss >> op;

        // -------- Core 00–4B --------
        if(line.find("00 nop")!=std::string::npos){
            out << "  ; nop\n";
        }
        else if(line.find("01 alloca")!=std::string::npos){
            out << "  " << newTmp() << " = alloca i32\n";
        }
        else if(line.find("02 load")!=std::string::npos){
            out << "  " << newTmp() << " = load i32, i32* %ptr\n";
        }
        else if(line.find("03 store")!=std::string::npos){
            out << "  store i32 0, i32* %ptr\n";
        }
        else if(line.find("15 icmp")!=std::string::npos){
            out << "  " << newTmp() << " = icmp slt i32 %a, %b\n";
        }
        else if(line.find("16 fcmp")!=std::string::npos){
            out << "  " << newTmp() << " = fcmp olt double %fa, %fb\n";
        }
        else if(line.find("17 add")!=std::string::npos){
            out << "  " << newTmp() << " = add i32 %a, %b\n";
        }
        else if(line.find("18 sub")!=std::string::npos){
            out << "  " << newTmp() << " = sub i32 %a, %b\n";
        }
        else if(line.find("19 mul")!=std::string::npos){
            out << "  " << newTmp() << " = mul i32 %a, %b\n";
        }
        else if(line.find("1A udiv")!=std::string::npos){
            out << "  " << newTmp() << " = udiv i32 %a, %b\n";
        }
        else if(line.find("1B sdiv")!=std::string::npos){
            out << "  " << newTmp() << " = sdiv i32 %a, %b\n";
        }
        else if(line.find("2B call")!=std::string::npos){
            out << "  call void @fn()\n";
        }
        else if(line.find("30 br")!=std::string::npos){
            out << "  br i1 %cond, label %iftrue, label %iffalse\n";
        }
        else if(line.find("33 ret")!=std::string::npos){
            out << "  ret i32 0\n";
        }

        // -------- Safety 50–7B --------
        else if(line.find("50 safe.add")!=std::string::npos){
            out << "  ; safe.add with overflow check\n";
            out << "  %ov = call {i32,i1} @llvm.sadd.with.overflow.i32(i32 %a, i32 %b)\n";
        }
        else if(line.find("51 safe.sub")!=std::string::npos){
            out << "  ; safe.sub\n";
            out << "  %ov = call {i32,i1} @llvm.ssub.with.overflow.i32(i32 %a, i32 %b)\n";
        }
        else if(line.find("53 safe.div")!=std::string::npos){
            out << "  ; guarded div\n";
            out << "  %zero = icmp eq i32 %b, 0\n";
            out << "  br i1 %zero, label %diverr, label %divok\n";
        }
        else if(line.find("7B language.assert")!=std::string::npos){
            out << "  ; assert\n";
            out << "  br i1 %cond, label %assert_ok, label %assert_fail\n";
        }

        // -------- Data Structures 80–9B --------
        else if(line.find("80 tuple.pack")!=std::string::npos){
            out << "  ; tuple.pack -> call runtime\n";
            out << "  call void @runtime_tuple_pack()\n";
        }
        else if(line.find("82 list.append")!=std::string::npos){
            out << "  call void @runtime_list_append()\n";
        }
        else if(line.find("86 array.load")!=std::string::npos){
            out << "  " << newTmp() << " = call i32 @runtime_array_load()\n";
        }
        else if(line.find("87 array.store")!=std::string::npos){
            out << "  call void @runtime_array_store()\n";
        }

        // -------- CIAM A0–BB --------
        else if(line.find("A6 echo")!=std::string::npos){
            out << "  call i32 @puts(i8* getelementptr ([20 x i8], [20 x i8]* @.str, i32 0, i32 0))\n";
        }
        else if(line.find("B9 language.sync")!=std::string::npos){
            out << "  ; sync barrier\n";
            out << "  call void @runtime_sync()\n";
        }
        else if(line.find("BB language.exit")!=std::string::npos){
            out << "  call void @exit(i32 0)\n";
        }

        // -------- Unknown --------
        else {
            out << "  ; unhandled DGM: " << line << "\n";
        }
    }

    out << "}\n";
}
