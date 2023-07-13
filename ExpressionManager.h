#ifndef EXPR_MANAGER_H
#define EXPR_MANAGER_H

#include <vector>

#include "Expression.h"

// Per risolvere il problema delle possibili "perdite di memoria"
// creo un gestore dei nodi che provvede alla loro deallocazione
class ExpressionManager {
public:
    // Il costruttore di default va bene perché invoca il costruttore
    // di default del vector che costruisce un vector vuoto
    ExpressionManager() = default;
    ~ExpressionManager() {
        clearMemory();
    }

    // Impedisco la copia e l'assegnamento per evitare problemi
    // di "ownership" dei puntatori (se facessi una copia di 
    // un oggetto ExpressionManager andrei incontro a deallocazioni multiple
    ExpressionManager(const ExpressionManager& other) = delete;
    ExpressionManager& operator=(const ExpressionManager& other) = delete;

    // Factory method per la creazione di oggetti Expression
    Expression* makeNumber(int value) {
        Expression* n = new Number(value);
        // Il metodo push_back aggiunge un elemento al vector allocated
        allocated.push_back(n);
        return n;
    }
    Expression* makeOperator(Operator::OpCode op, Expression* l, Expression* r) {
        Expression* o = new Operator(op, l, r);
        allocated.push_back(o);
        return o;
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
    std::vector<Expression*> allocated;
};



#endif