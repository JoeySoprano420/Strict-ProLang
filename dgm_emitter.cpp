#include "dgm_emitter.hpp"
#include <fstream>
#include <sstream>

// ===== Expressions =====
std::string DGMEmitter::genExpr(Expr* e) {
    if (auto n = dynamic_cast<NumberExpr*>(e)) {
        std::ostringstream oss;
        oss << n->value;
        return oss.str();
    }
    if (auto s = dynamic_cast<StringExpr*>(e)) {
        return "\"" + s->text + "\"";
    }
    if (auto v = dynamic_cast<VarExpr*>(e)) {
        return v->name;
    }
    if (auto b = dynamic_cast<BinaryExpr*>(e)) {
        std::string l = genExpr(b->lhs);
        std::string r = genExpr(b->rhs);
        std::string t = newTmp();

        if (b->op == "+") emit("17 add " + l + ", " + r + " -> " + t);
        else if (b->op == "-") emit("18 sub " + l + ", " + r + " -> " + t);
        else if (b->op == "*") emit("19 mul " + l + ", " + r + " -> " + t);
        else if (b->op == "/") emit("1B sdiv " + l + ", " + r + " -> " + t);
        else if (b->op == ">") emit("15 icmp " + l + " > " + r + " -> " + t);
        else if (b->op == "<") emit("15 icmp " + l + " < " + r + " -> " + t);
        else if (b->op == "=") emit("15 icmp " + l + " = " + r + " -> " + t);

        return t;
    }
    if (auto c = dynamic_cast<CallExpr*>(e)) {
        std::ostringstream oss;
        oss << "call " << c->callee << "(";
        for (size_t i = 0; i < c->args.size(); i++) {
            if (i) oss << ", ";
            oss << genExpr(c->args[i]);
        }
        oss << ")";
        emit("2B call " + oss.str());
        return newTmp();
    }
    return "undef";
}

// ===== Statements =====
void DGMEmitter::genStmt(Stmt* s) {
    if (auto let = dynamic_cast<LetStmt*>(s)) {
        std::string val = genExpr(let->expr);
        emit("03 store " + val + " -> " + let->name);
    }
    else if (auto pr = dynamic_cast<PrintStmt*>(s)) {
        std::string val = genExpr(pr->expr);
        emit("A6 echo " + val);
    }
    else if (auto iff = dynamic_cast<IfStmt*>(s)) {
        std::string cond = genExpr(iff->cond);
        emit("; If condition");
        emit("15 icmp " + cond);
        emit("30 br iftrue, iffalse");

        emit("; Then branch");
        for (auto ss : iff->thenBody) genStmt(ss);

        if (!iff->elseBody.empty()) {
            emit("; Else branch");
            for (auto ss : iff->elseBody) genStmt(ss);
        }
        emit("EndIf");
    }
    else if (auto ret = dynamic_cast<ReturnStmt*>(s)) {
        std::string val = genExpr(ret->expr);
        emit("33 ret " + val);
    }
    else if (auto f = dynamic_cast<FuncStmt*>(s)) {
        emit("; Function " + f->name);
        emit("FuncBegin " + f->name);
        for (auto ss : f->body) genStmt(ss);
        emit("FuncEnd " + f->name);
    }
    else if (auto c = dynamic_cast<ClassStmt*>(s)) {
        emit("; Class " + c->name);
        emit("ClassBegin " + c->name);
        for (auto ss : c->body) genStmt(ss);
        emit("ClassEnd " + c->name);
    }
    else if (auto m = dynamic_cast<MatchStmt*>(s)) {
        emit("; Match expression");
        std::string tgt = genExpr(m->target);
        emit("95 match.begin " + tgt);
        for (auto& cs : m->cases) {
            std::string pat = genExpr(cs.pattern);
            emit("96 match.case " + pat);
            for (auto ss : cs.body) genStmt(ss);
        }
        emit("97 match.end");
    }
}

// ===== Program =====
void DGMEmitter::gen(std::vector<Stmt*>& prog) {
    for (auto s : prog) genStmt(s);
}

void DGMEmitter::write(const std::string& path) {
    std::ofstream f(path);
    for (auto& line : code) f << line << "\n";
}
