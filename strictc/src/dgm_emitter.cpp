# strictc/src/dgm_emitter.cpp
#include "dgm_emitter.hpp"
#include <fstream>

std::string DGMEmitter::genExpr(Expr* e){
    if(auto n=dynamic_cast<NumberExpr*>(e)) return std::to_string(n->value);
    if(auto s=dynamic_cast<StringExpr*>(e)) return "\"" + s->text + "\"";
    if(auto v=dynamic_cast<VarExpr*>(e)) return v->name;
    if(auto b=dynamic_cast<BinaryExpr*>(e)){
        std::string l=genExpr(b->lhs), r=genExpr(b->rhs);
        if(b->op=="+") emit("17 add "+l+", "+r);
        if(b->op=="-") emit("18 sub "+l+", "+r);
        return "tmp";
    }
    return "expr";
}

void DGMEmitter::genStmt(Stmt* s){
    if(auto let=dynamic_cast<LetStmt*>(s)){
        std::string val=genExpr(let->expr);
        emit("03 store "+val+" -> "+let->name);
    }
    if(auto pr=dynamic_cast<PrintStmt*>(s)){
        std::string val=genExpr(pr->expr);
        emit("A6 echo "+val);
    }
    if(auto iff=dynamic_cast<IfStmt*>(s)){
        std::string cond=genExpr(iff->cond);
        emit("15 icmp "+cond);
        for(auto ss:iff->thenBody) genStmt(ss);
        if(!iff->elseBody.empty()){
            emit("Else");
            for(auto ss:iff->elseBody) genStmt(ss);
        }
        emit("EndIf");
    }
}

void DGMEmitter::gen(std::vector<Stmt*>& prog){
    for(auto s:prog) genStmt(s);
}

void DGMEmitter::write(const std::string& path){
    std::ofstream f(path);
    for(auto& line:code) f<<line<<"\n";
}
