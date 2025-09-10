#include "dgm_translator.hpp"
#include "runtime.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>

static int tmpCount = 0;
static std::string newTmp() { return "%t" + std::to_string(++tmpCount); }

void DGMTranslator::translate(const std::string& inFile, const std::string& outFile) {
    std::ifstream in(inFile);
    std::ofstream out(outFile);
    std::string line;

    // Table of tasks discovered
    std::set<std::string> taskNames;
    std::unordered_map<std::string,std::vector<std::string>> taskBodies;

    // === LLVM Preamble ===
    out << "; DGM → LLVM IR Translator Output\n";
    out << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

    // Declarations (runtime + libc)
    out << "declare i32 @puts(i8*)\n";
    out << "declare i32 @scanf(i8*, ...)\n";
    out << "declare void @exit(i32)\n";
    out << "declare void @runtime_print(i8*)\n";
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
    out << "declare i64 @runtime_future(void ()*)\n";
    out << "declare void @runtime_parallel(void ()*, void ()*)\n";
    out << "declare void @runtime_sync()\n";
    out << "declare void @runtime_yield()\n";
    out << "declare void @runtime_resume()\n";
    out << "declare void @runtime_halt()\n\n";

    // IR constants (for echo strings)
    out << "@.str = private unnamed_addr constant [32 x i8] c\"Hello from Main\\00\", align 1\n";
    out << "@.str1 = private unnamed_addr constant [32 x i8] c\"Hello from Task\\00\", align 1\n";

    // === Main entry ===
    out << "define i32 @main() {\n";

    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string op;
        iss >> op;

        // === Core Ops Example ===
        if (line.find("03 store") != std::string::npos) {
            out << "  store i32 0, i32* %ptr\n";
        }
        else if (line.find("A6 echo") != std::string::npos) {
            out << "  call i32 @puts(i8* getelementptr ([20 x i8], [20 x i8]* @.str, i32 0, i32 0))\n";
        }
        else if (line.find("33 ret") != std::string::npos) {
            out << "  ret i32 0\n";
        }

        // === Future Task Registration ===
        else if (line.find("B7 language.future") != std::string::npos) {
            std::string name;
            iss >> op; // skip B7
            iss >> op; // skip "language.future"
            iss >> name; // task name
            taskNames.insert(name);
            out << "  ; spawn future " << name << "\n";
            out << "  call i64 @runtime_future(void ()* @" << name << "_fn)\n";
        }

        // === Parallel Task Registration ===
        else if (line.find("B8 language.parallel") != std::string::npos) {
            std::string t1, t2;
            iss >> op; // skip B8
            iss >> op; // skip "language.parallel"
            iss >> t1; // first task
            iss >> t2; // second task
            taskNames.insert(t1);
            taskNames.insert(t2);
            out << "  ; run " << t1 << " and " << t2 << " in parallel\n";
            out << "  call void @runtime_parallel(void ()* @" << t1 << "_fn, void ()* @" << t2 << "_fn)\n";
        }

        // === Sync ===
        else if (line.find("B9 language.sync") != std::string::npos) {
            out << "  call void @runtime_sync()\n";
        }

        // === Exit ===
        else if (line.find("BB language.exit") != std::string::npos) {
            out << "  call void @runtime_halt()\n";
        }

        // Fallback
        else {
            out << "  ; unhandled DGM: " << line << "\n";
        }
    }

    out << "  ret i32 0\n";
    out << "}\n\n";

    // === Auto-generate task functions ===
    for (auto& name : taskNames) {
        out << "define void @" << name << "_fn() {\n";
        out << "  ; body of " << name << "\n";
        out << "  call i32 @puts(i8* getelementptr ([32 x i8], [32 x i8]* @.str1, i32 0, i32 0))\n";
        out << "  ret void\n";
        out << "}\n\n";
    }
}
