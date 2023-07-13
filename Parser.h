#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Exceptions.h"
#include "Token.h"
#include "Expression.h"
#include "ExpressionManager.h"

// Function object per il parsing di espressioni
// Funzione di parsing: restituisce "true" se l'espressione è corretta
// "false" altrimenti. Suppone che nella stringa vi siano solo simboli
// terminali per la grammatica definita sopra.
class ParseExpression {

public:
    ParseExpression(ExpressionManager& manager) : em{ manager } { }

    Expression* operator()(const std::vector<Token>& tokenStream) {
        // Utilizzo auto per evitare di scrivere per esteso il tipo dell'iteratore
        auto tokenItr = tokenStream.begin();
        streamEnd = tokenStream.end();
        Expression* expr = recursiveParse(tokenItr);
        // TODO: controllare se sono arrivato in fondo al token stream
        return expr;
    }

private:
    std::vector<Token>::const_iterator streamEnd;

    // Riferimento all'expression manager "di sistema"
    ExpressionManager& em;

    // Parser a discesa ricorsiva nella struttura dell'espressione
    Expression* recursiveParse(std::vector<Token>::const_iterator& tokenItr);

    // Avanzamento "sicuro" di un iteratore
    void safe_next(std::vector<Token>::const_iterator& itr) {
        if (itr == streamEnd) {
            throw ParseError("Unexpected end of input");
        }
        ++itr;
    }
};

#endif