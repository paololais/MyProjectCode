#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include "NumExpr.h"
#include "Node.h"

#include <string>

// Dichiarazione "forward" della classe Visitor
// La classe è implementata in un altro modulo
class Visitor;

class BoolExpr : public Node{
public:
    virtual ~BoolExpr() {};
    virtual void accept(Visitor* v) = 0;
};

// Classe derivata per espressioni numeriche contenenti un numero
// Si tratta di un oggetto immutabile: assegno un valore al campo
// value all'atto della costruzione e poi posso solo accedere al valore
class BoolOp : public BoolExpr {
public:
    enum OpCode { NOT, AND, OR };
    
    BoolOp(OpCode o, BoolExpr* b) : op{ o }, boolExpr1{ b }, boolExpr2{nullptr} { }
    BoolOp(OpCode o, BoolExpr* lb, BoolExpr* rb) : op{ o }, boolExpr1{lb}, boolExpr2{rb}{ }

    ~BoolOp() = default;

    BoolOp(const BoolOp& other) = default;
    BoolOp& operator=(const BoolOp& other) = default;

    OpCode getOpCode() const {
        return op;
    }

    BoolExpr* getBoolExpr1(){
        return boolExpr1;
    }
    BoolExpr* getBoolExpr2(){
        return boolExpr2;
    }

    void accept(Visitor* v) override;

private:
    OpCode op;
    BoolExpr* boolExpr1;
    BoolExpr* boolExpr2;
};

// Classe derivata per espressioni numeriche contenenti un operatore
// Anche questi sono oggetti immutabili
class RelOp : public BoolExpr {
public:
    enum OpCode { GT, LT, EQ };
     
    RelOp(OpCode o, NumExpr* lop, NumExpr* rop) : 
        op{ o }, left{ lop }, right{ rop }{ }

    // Il distruttore di default implica che la deallocazione
    // degli operandi � responsabilit� di qualcun altro
    // Questo � coerente con il costruttore che chiede nodi gi� allocati
    ~RelOp() = default;

    // La copia e l'assegnamento sono "superficiali": la struttura
    // profonda degli operandi non viene copiata
    RelOp(const RelOp& other) = default;
    RelOp& operator=(const RelOp& other) = default;

    // Metodi di accesso
    OpCode getOp() const {
        return op;
    }
    NumExpr* getLeft() const {
        return left;
    }
    NumExpr* getRight() const {
        return right;
    }

   void accept(Visitor* v) override;

private:
    OpCode      op;
    NumExpr* left;
    NumExpr* right;

};

class BoolConst : public NumExpr {
public:
    BoolConst(bool b) : boolConst{ b } { }
    ~BoolConst() = default;

    BoolConst(const BoolConst& other) = default;
    BoolConst& operator=(const BoolConst& other) = default;
    
    bool getBoolConst(){
        return boolConst;
    }

    void accept(Visitor* v) override;

private:
    bool boolConst;
};

#endif
