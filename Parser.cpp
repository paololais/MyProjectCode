#include <sstream>

#include "Parser.h"
#include "Token.h"
#include "Exceptions.h"

// Parser a discesa ricorsiva nella struttura dell'espressione
Expression* ParseExpression::recursiveParse(std::vector<Token>::const_iterator& itr) {
    // Un'espressione corretta inizia per un numero oppure Token::LP
    if (itr->tag == Token::LP) {
        // Devo analizzare un'espressione "composita" (E1 @ E2) @ = {+,-,*,/}
        // Con recursiveParse analizzo E1 e mi trovo l'iteratore su @
        safe_next(itr);
        Expression* left = recursiveParse(itr);
        Operator::OpCode op;
        switch (itr->tag) {
        case Token::ADD: op = Operator::PLUS; break;
        case Token::SUB: op = Operator::MINUS; break;
        case Token::MUL: op = Operator::TIMES; break;
        case Token::DIV: op = Operator::DIV; break;
        default:
            throw ParseError("Unrecognized operator");
        }
        // Avanzo a E2
        safe_next(itr);
        // Con recursiveParse analizzo E2 e mi trovo l'iteratore su ')'
        Expression* right = recursiveParse(itr);
        // Controllo che alla posizione corrente ci sia una ')'
        if (itr->tag != Token::RP) {
            throw ParseError("Mismatched parenthesis");
        }
        safe_next(itr);
        return em.makeOperator(op, left, right);
    } else if (itr->tag == Token::NUM) {
        std::stringstream temp{};
        temp << itr->word;
        int value;
        temp >> value;
        Expression* expr = em.makeNumber(value);
        safe_next(itr);
        return expr;
    } else {
        throw ParseError("Unexpected token");
        return nullptr; 
    }
}
