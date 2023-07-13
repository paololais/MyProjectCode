#ifndef EXPRESSION_H
#define EXPRESSION_H

// Dichiarazione "forward" della classe Visitor
// La classe è implementata in un altro modulo
class Visitor;

// Classe base astratta Expression (Composite + Interpreter)
// Supponiamo di voler valutare espressioni aritmetiche intere
class Expression {
public:
    virtual ~Expression() {};
    virtual void accept(Visitor* v) = 0;
};

// Classe derivata per espressioni numeriche contenenti un numero
// Si tratta di un oggetto immutabile: assegno un valore al campo
// value all'atto della costruzione e poi posso solo accedere al valore
class Number : public Expression {
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
class Operator : public Expression {
public:
    enum OpCode { PLUS, MINUS, TIMES, DIV };

    Operator(OpCode o, Expression* lop, Expression* rop) :
        op{ o }, left{ lop }, right{ rop } { }
    // Il distruttore di default implica che la deallocazione
    // degli operandi è responsabilità di qualcun altro
    // Questo è coerente con il costruttore che chiede nodi già allocati
    ~Operator() = default;

    // La copia e l'assegnamento sono "superficiali": la struttura
    // profonda degli operandi non viene copiata
    Operator(const Operator& other) = default;
    Operator& operator=(const Operator& other) = default;

    // Metodi di accesso
    OpCode getOp() const {
        return op;
    }
    Expression* getLeft() const {
        return left;
    }
    Expression* getRight() const {
        return right;
    }

    void accept(Visitor* v) override;

    // Metodo di utilità per convertire caratteri in OpCode
    // Metodo "a livello di classe": non opera su un particolare oggetto
    static OpCode charToOpCode(char ch) {
        switch (ch) {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return TIMES;
        case '/': return DIV;
        }
    }
    static char opCodeTochar(OpCode op) {
        switch (op) {
        case PLUS: return '+';
        case MINUS: return '-';
        case TIMES: return '*';
        case DIV: return '/';
        }
    }

private:
    OpCode      op;
    Expression* left;
    Expression* right;

};


#endif