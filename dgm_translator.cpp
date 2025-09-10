# strictc/src/dgm_translator.cpp
#include "dgm_translator.hpp"
#include "runtime.h"
#include <fstream>
#include <sstream>
#include <iostream>
# strictc/src/dgm_translator.cpp

...
    // CIAM A0–BB
    else if(line.find("A6 echo")!=std::string::npos){
        out << "  call i32 @puts(i8* getelementptr ([20 x i8], [20 x i8]* @.str, i32 0, i32 0))\n";
    }
    else if(line.find("B7 language.future")!=std::string::npos){
        out << "  ; spawn future thread\n";
        out << "  call i64 @runtime_future(void ()* @task_fn)\n";
    }
    else if(line.find("B8 language.parallel")!=std::string::npos){
        out << "  ; run two tasks in parallel\n";
        out << "  call void @runtime_parallel(void ()* @task1_fn, void ()* @task2_fn)\n";
    }
    else if(line.find("B9 language.sync")!=std::string::npos){
        out << "  call void @runtime_sync()\n";
    }
    else if(line.find("BB language.exit")!=std::string::npos){
        out << "  call void @runtime_halt()\n";
    }
...


void DGMTranslator::translate(const std::string& inFile,const std::string& outFile){
    std::ifstream in(inFile);
    std::ofstream out(outFile);
    std::string line;

    // === LLVM IR Preamble ===
    out << "; StrictC LLVM IR generated from DGM\n";
    out << "declare i32 @puts(i8*)\n";
    out << "declare i32 @scanf(i8*, ...)\n";
    out << "declare void @runtime_print(i8*)\n";

    // Runtime hooks
    out << "declare %struct.Tuple* @runtime_tuple_pack(i32, i8**)\n";
    out << "declare i8** @runtime_tuple_unpack(%struct.Tuple*)\n";
    out << "declare %struct.List* @runtime_list_new()\n";
    out << "declare void @runtime_list_append(%struct.List*, i32)\n";
    out << "declare void @runtime_list_remove(%struct.List*, i32)\n";
    out << "declare void @runtime_list_insert(%struct.List*, i32, i32)\n";
    out << "declare i32 @runtime_list_pop(%struct.List*)\n";
    out << "declare %struct.Array* @runtime_array_new(i32)\n";
    out << "declare i32 @runtime_array_load(%struct.Array*, i32)\n";
    out << "declare void @runtime_array_store(%struct.Array*, i32, i32)\n";
    out << "declare %struct.Group* @runtime_group_spawn()\n";
    out << "declare void @runtime_group_merge(%struct.Group*, i8*)\n";
    out << "declare void @runtime_group_split(%struct.Group*)\n";
    out << "declare void @runtime_sync()\n";
    out << "declare void @runtime_yield()\n";
    out << "declare void @runtime_resume()\n";
    out << "declare void @runtime_halt()\n";

    out << "define i32 @main() {\n";

    int tmpCount = 0;
    auto newTmp = [&](){ return "%t" + std::to_string(++tmpCount); };

    while(std::getline(in,line)){
        std::istringstream iss(line);
        std::string op; iss >> op;

        // Core 00–4B
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
        else if(line.find("17 add")!=std::string::npos){
            out << "  " << newTmp() << " = add i32 %a, %b\n";
        }
        else if(line.find("18 sub")!=std::string::npos){
            out << "  " << newTmp() << " = sub i32 %a, %b\n";
        }
        else if(line.find("19 mul")!=std::string::npos){
            out << "  " << newTmp() << " = mul i32 %a, %b\n";
        }
        else if(line.find("1B sdiv")!=std::string::npos){
            out << "  " << newTmp() << " = sdiv i32 %a, %b\n";
        }
        else if(line.find("33 ret")!=std::string::npos){
            out << "  ret i32 0\n";
        }

        // Safety 50–7B
        else if(line.find("50 safe.add")!=std::string::npos){
            out << "  %ov = call {i32,i1} @llvm.sadd.with.overflow.i32(i32 %a, i32 %b)\n";
        }
        else if(line.find("53 safe.div")!=std::string::npos){
            out << "  %zero = icmp eq i32 %b, 0\n";
            out << "  br i1 %zero, label %diverr, label %divok\n";
        }
        else if(line.find("7B language.assert")!=std::string::npos){
            out << "  br i1 %cond, label %assert_ok, label %assert_fail\n";
        }

        // Data Structures 80–9B
        else if(line.find("80 tuple.pack")!=std::string::npos){
            out << "  %args = alloca i8*\n";
            out << "  " << newTmp() << " = call %struct.Tuple* @runtime_tuple_pack(i32 2, i8** %args)\n";
        }
        else if(line.find("82 list.append")!=std::string::npos){
            out << "  call void @runtime_list_append(%struct.List* %list, i32 42)\n";
        }
        else if(line.find("83 list.remove")!=std::string::npos){
            out << "  call void @runtime_list_remove(%struct.List* %list, i32 0)\n";
        }
        else if(line.find("86 array.load")!=std::string::npos){
            out << "  " << newTmp() << " = call i32 @runtime_array_load(%struct.Array* %arr, i32 0)\n";
        }
        else if(line.find("87 array.store")!=std::string::npos){
            out << "  call void @runtime_array_store(%struct.Array* %arr, i32 0, i32 123)\n";
        }

        // CIAM A0–BB
        else if(line.find("A6 echo")!=std::string::npos){
            out << "  call i32 @puts(i8* getelementptr ([20 x i8], [20 x i8]* @.str, i32 0, i32 0))\n";
        }
        else if(line.find("B9 language.sync")!=std::string::npos){
            out << "  call void @runtime_sync()\n";
        }
        else if(line.find("BB language.exit")!=std::string::npos){
            out << "  call void @runtime_halt()\n";
        }

        // Unknown
        else {
            out << "  ; unhandled DGM: " << line << "\n";
        }
    }

    out << "}\n";
}
