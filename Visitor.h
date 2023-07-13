#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <iostream>

#include "Expression.h"

// Visitor astratto per la visita degli oggetti della gerarchia "Expression"
class Visitor {
public:
    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
};

// Visitor concreto per la valutazione delle espressioni
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor() : accumulator{ } { }
    // TODO: distruttore, costruttori di copia, ecc.

    void visitOperator(Operator* opNode) override {
        // Prelevo i puntatori agli operandi e gli faccio accettare 
        // questo oggetto come visitatore (propago la visita)
        Expression* left = opNode->getLeft();
        left->accept(this);
        Expression* right = opNode->getRight();
        right->accept(this);
        // Prelevo i valori dall'accumulatore
        int rval = accumulator.back(); accumulator.pop_back();
        int lval = accumulator.back(); accumulator.pop_back();
        switch (opNode->getOp()) {
        case Operator::PLUS:
            accumulator.push_back(lval + rval); return;
        case Operator::MINUS:
            accumulator.push_back(lval - rval); return;
        case Operator::TIMES:
            accumulator.push_back(lval * rval); return;
        case Operator::DIV:
            accumulator.push_back(lval / rval); return;
        default:
            return;
        }
    }
    void visitNumber(Number* numNode) override {
        // Il metodo push_back inserisce il valore come ultimo elemento del vector
        // (equivale ad una PUSH in uno stack)
        accumulator.push_back(numNode->get_Value());
    }
    int getValue() const {
        // Il metodo back() restituisce l'ultimo valore del vector 
        // (equivale a una TOP in uno stack) 
        return accumulator.back();
    }
private:
    std::vector<int> accumulator;

};


// Visitor concreto per la stampa delle espressioni
class PrintVisitor : public Visitor {
public:
    void visitOperator(Operator* opNode) override {
        std::cout << "(";
        opNode->getLeft()->accept(this);
        std::cout << Operator::opCodeTochar(opNode->getOp());
        opNode->getRight()->accept(this);
        std::cout << ")";
    }
    void visitNumber(Number* numNode) override {
        std::cout << numNode->get_Value();
    }
};


#endif