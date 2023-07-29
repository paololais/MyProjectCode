#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include <vector>

#include "Node.h"
#include "StmtBlock.h"
#include "Block.h"
#include "Program.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Statement.h"

// Per risolvere il problema delle possibili "perdite di memoria"
// creo un gestore dei nodi che provvede alla loro deallocazione
class NodeManager {
public:
    // Il costruttore di default va bene perch� invoca il costruttore
    // di default del vector che costruisce un vector vuoto
    NodeManager() = default;
    ~NodeManager() {
        clearMemory();
    }

    // Impedisco la copia e l'assegnamento per evitare problemi
    // di "ownership" dei puntatori (se facessi una copia di 
    // un oggetto ExpressionManager andrei incontro a deallocazioni multiple
    NodeManager(const NodeManager& other) = delete;
    NodeManager& operator=(const NodeManager& other) = delete;

     // factory method per gestione Program
    Node* makeProgram(StmtBlock* b) {
        Node* p = new Program(b);
        allocated.push_back(p);
        return p;
    }

     // factory method per gestione StmtBlock
    Node* makeStmtBlock(Block* b) {
        Node* p = new StmtBlock(b);
        allocated.push_back(p);
        return p;
    }
    Node* makeStmtBlock(Statement* stmt) {
        Node* p = new StmtBlock(stmt);
        allocated.push_back(p);
        return p;
    }
    
    // factory method per gestione Block
    Node* makeBlock(const std::vector<Statement*>& s) {
        Node* b = new Block(s);
        allocated.push_back(b);
        return b;
    }

    // factory method per gestione Statement
    Node* makePrintStmt(NumExpr* n) {
        Node* s = new PrintStmt(n);
        allocated.push_back(s);
        return s;
    }

    Node* makeSetStmt(Variable* v, NumExpr* n) {
        Node* s = new SetStmt(n, v);
        allocated.push_back(s);
        return s;
    }

    Node* makeInputStmt(Variable* v) {
        Node* s = new InputStmt(v);
        allocated.push_back(s);
        return s;
    }

    Node* makeWhileStmt(BoolExpr* b, StmtBlock* blc) {
        Node* s = new WhileStmt(b, blc);
        allocated.push_back(s);
        return s;
    }

    Node* makeIfStmt(BoolExpr* b, StmtBlock* blc1, StmtBlock* blc2) {
        Node* s = new IfStmt(b, blc1, blc2);
        allocated.push_back(s);
        return s;
    }

    // factory method per gestione NumExpr
    Node* makeNumber(int value) {
        Node* n = new Number(value);
        // Il metodo push_back aggiunge un elemento al vector allocated
        allocated.push_back(n);
        return n;
    }

    Node* makeOperator(Operator::OpCode op, NumExpr* l, NumExpr* r) {
        Node* o = new Operator(op, l, r);
        allocated.push_back(o);
        return o;
    }

    Node* makeVariable(std::string s) {
        Node* v = new Variable(s);
        allocated.push_back(v);
        return v;
    }

    // factory method per gestione BoolExpr
    Node* makeRelOp(RelOp::OpCode op, NumExpr* l, NumExpr* r){
        Node* rn = new RelOp(op, l, r);
        allocated.push_back(rn);
        return rn;
    }

    Node* makeBoolOp(BoolOp::OpCode op, BoolExpr* l, BoolExpr* r){
        Node* rb = new BoolOp(op, l, r);
        allocated.push_back(rb);
        return rb;
    }

    Node* makeBoolConst(bool b){
        Node* bc = new BoolConst(b);
        allocated.push_back(bc);
        return bc;
    }

    Node* makeBoolOpNot(BoolOp::OpCode o, BoolExpr* b){
        Node* bo = new BoolOp(o, b);
        allocated.push_back(bo);
        return bo;
    }

    void clearMemory() {
        auto i = allocated.begin();
        // allocated.end() "marca" la fine del vettore
        // ++i sposta di un elemento la posizione "puntata" dall'iteratore
        for (; i != allocated.end(); ++i) {
            // *i restituisce l'elemento "puntato" dall'iteratore
            delete(*i);
        }
        allocated.resize(0);
    }

private:
    std::vector<Node*> allocated;
};
#endif