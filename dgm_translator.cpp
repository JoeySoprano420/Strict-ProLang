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

void DGMTranslator::translate(const std::string& inFile,const std::string& outFile) {
    std::ifstream in(inFile);
    std::ofstream out(outFile);
    std::string line;

    // === State ===
    std::set<std::string> taskNames;
    std::unordered_map<std::string,std::vector<std::string>> taskBodies;
    std::unordered_map<std::string,std::string> stringPool; // literal -> @.strN
    bool insideTask = false;
    std::string currentTask;
    int strCount = 0;

    // === LLVM Preamble ===
    out << "; DGM → LLVM IR Translator Output\n";
    out << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

    // Runtime declarations
    out << "declare i32 @puts(i8*)\n";
    out << "declare void @runtime_halt()\n";
    out << "declare void @runtime_sync()\n";
    out << "declare i64 @runtime_future(void ()*)\n";
    out << "declare void @runtime_parallel(void ()*, void ()*)\n";
    out << "declare void @runtime_list_append(%struct.List*, i32)\n\n";

    // === Main entry ===
    out << "define i32 @main() {\n";

    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string op; iss >> op;

        // === Future Task Begin ===
        if (line.find("B7 language.future") != std::string::npos) {
            std::string taskName;
            iss >> op; iss >> op; // skip B7 + "language.future"
            iss >> taskName;
            taskNames.insert(taskName);
            currentTask = taskName;
            insideTask = true;
            continue;
        }

        // === EndTask ===
        if (line.find("EndTask") != std::string::npos) {
            insideTask = false;
            currentTask.clear();
            continue;
        }

        // === Parallel ===
        if (line.find("B8 language.parallel") != std::string::npos) {
            std::string t1, t2;
            iss >> op; iss >> op; // skip B8 + "language.parallel"
            iss >> t1 >> t2;
            out << "  call void @runtime_parallel(void ()* @" << t1 << "_fn, void ()* @" << t2 << "_fn)\n";
            continue;
        }

        // === Exit ===
        if (line.find("BB language.exit") != std::string::npos) {
            out << "  call void @runtime_halt()\n";
            continue;
        }

        // === Echo (Print) ===
        if (line.find("A6 echo") != std::string::npos) {
            // Extract literal
            size_t pos = line.find("\"");
            std::string lit;
            if (pos != std::string::npos) {
                lit = line.substr(pos); // from first quote
            }

            if (stringPool.find(lit) == stringPool.end()) {
                std::ostringstream name;
                name << "@.str" << (++strCount);
                stringPool[lit] = name.str();
            }
            std::string globalName = stringPool[lit];

            std::ostringstream instr;
            instr << "  call i32 @puts(i8* getelementptr (["
                  << lit.size() << " x i8], [" << lit.size() << " x i8]* "
                  << globalName << ", i32 0, i32 0))";

            if (insideTask) taskBodies[currentTask].push_back(instr.str());
            else out << instr.str() << "\n";
            continue;
        }

        // === Store (Let) ===
        if (line.find("03 store") != std::string::npos) {
            std::string instr = "  store i32 0, i32* %ptr";
            if (insideTask) taskBodies[currentTask].push_back(instr);
            else out << instr << "\n";
            continue;
        }

        // === Fallback ===
        if (insideTask) taskBodies[currentTask].push_back("  ; " + line);
        else out << "  ; unhandled DGM: " << line << "\n";
    }

    out << "  ret i32 0\n";
    out << "}\n\n";

    // === Emit string constants ===
    for (auto& kv : stringPool) {
        const std::string& lit = kv.first;
        const std::string& name = kv.second;
        int len = (int)lit.size(); // includes quotes
        std::string raw = lit.substr(1, lit.size() - 2); // remove quotes
        out << name << " = private unnamed_addr constant ["
            << len << " x i8] c\"" << raw << "\\00\", align 1\n";
    }
    out << "\n";

    // === Emit task functions ===
    for (auto& name : taskNames) {
        out << "define void @" << name << "_fn() {\n";
        for (auto& instr : taskBodies[name]) {
            out << instr << "\n";
        }
        out << "  ret void\n";
        out << "}\n\n";
    }
}
