#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <variant>
#include <iostream>
#include <unordered_map>

#include "Program.h"
#include "StmtBlock.h"
#include "Statement.h"
#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Exceptions.h"

// Visitor concreto per la valutazione delle espressioni
class Visitor{
public:
    Visitor() : intAccumulator{}, boolAccumulator{}, symbolTable{}, isInSetContext(false) {}

    // TODO: distruttore, costruttori di copia, ecc.
    ~Visitor() = default;

    void visitProgram(Program* progrNode);
    void visitStmtBlock(StmtBlock* stmtblockNode);
    void visitBlock(Block* blockNode);
    
    // Statement
    void visitPrintStmt(PrintStmt* statement);
    void visitSetStmt(SetStmt* statement);
    void visitInputStmt(InputStmt* statement);
    void visitWhileStmt(WhileStmt* statement);
    void visitIfStmt(IfStmt* statement);

    // NumExpr
    void visitOperator(Operator* opNode);
    void visitNumber(Number* numNode);
    void visitVariable(Variable* varNode);
    
    // BoolExpr
    void visitBoolOp(BoolOp* boolNode);
    void visitRelOp(RelOp* relNode);
    void visitBoolConst(BoolConst* boolconst);

    //metodi per safe access ai vector, nel caso fossero vuoti -> errore
    int getBackIntAccumulator(){
        if (!intAccumulator.empty())
        {
            return intAccumulator.back();
        } else {
            throw EvalError("Trying to access empty intAccumulator vector");
        }
    }
    int getBackBoolAccumulator(){
        if (!boolAccumulator.empty())
        {
            return boolAccumulator.back();
        } else {
            throw EvalError("Trying to access empty boolAccumulator vector");
        }
    }

private:
    // Tabella dei simboli per memorizzare i valori delle variabili
    std::unordered_map<std::string, int> symbolTable;

    //accumulatori per la valutazione delle espressioni nel programma
    std::vector<int> intAccumulator;
    std::vector<bool> boolAccumulator;

    //variabile booleana per tenere traccia se siamo nel contesto di una SET
    //utilizzo per generare errore o meno se stiamo utilizzando una variabile non definita
    bool isInSetContext;
};

#endif
