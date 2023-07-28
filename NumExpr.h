#ifndef NUMEXPR_H
#define NUMEXPR_H

#include <string>

#include "Node.h"

// Dichiarazione "forward" della classe Visitor
// La classe è implementata in un altro modulo
class Visitor;

// Classe base astratta NumExpr (Composite + Interpreter)
// Supponiamo di voler valutare espressioni aritmetiche intere
class NumExpr : public Node{
public:
    virtual ~NumExpr() {};

    virtual void accept(Visitor* v) = 0;
};

// Classe derivata per espressioni numeriche contenenti un numero
// Si tratta di un oggetto immutabile: assegno un valore al campo
// value all'atto della costruzione e poi posso solo accedere al valore
class Number : public NumExpr {
public:
    Number(int v) : value{ v } { }
    ~Number() = default;

    Number(const Number& other) = default;
    Number& operator=(const Number& other) = default;

    int get_Value() const {
        return value;
    }

    void accept(Visitor* v) override;

private:
    int value;
};

// Classe derivata per espressioni numeriche contenenti un operatore
// Anche questi sono oggetti immutabili
class Operator : public NumExpr {
public:
    enum OpCode { ADD, SUB, MUL, DIV };

    Operator(OpCode o, NumExpr* lop, NumExpr* rop) :
        op{ o }, left{ lop }, right{ rop } { }
    // Il distruttore di default implica che la deallocazione
    // degli operandi � responsabilit� di qualcun altro
    // Questo � coerente con il costruttore che chiede nodi gi� allocati
    ~Operator() = default;

    // La copia e l'assegnamento sono "superficiali": la struttura
    // profonda degli operandi non viene copiata
    Operator(const Operator& other) = default;
    Operator& operator=(const Operator& other) = default;

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

// Classe derivata per variabili
class Variable : public NumExpr {
public:
    Variable(std::string var, int val) : variable_id{ var }, value{ val } { }
    Variable(std::string var) : variable_id{ var } { }
    ~Variable() = default;

    Variable(const Variable& other) = default;
    Variable& operator=(const Variable& other) = default;

    std::string get_variable_id() const {
        return variable_id;
    }
    int get_value() const {
        return value;
    }

    void set_variable_id(std::string id){
        variable_id = id;
    }
    void set_value(int v){
        value = v;
    }

    void accept(Visitor* v) override;

private:
    std::string variable_id;
    int value;
};

#endif