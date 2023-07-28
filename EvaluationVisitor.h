#ifndef EVAL_VISITOR_H
#define EVAL_VISITOR_H

#include <vector>
#include <variant>
#include <iostream>
#include <unordered_map>

#include "Visitor.h"
#include "Program.h"
#include "StmtBlock.h"
#include "Statement.h"
#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Exceptions.h"

// Visitor concreto per la valutazione delle espressioni
class EvaluationVisitor : public Visitor {
public:
    EvaluationVisitor() : intAccumulator{}, boolAccumulator{}, symbolTable{}, isInSetContext(false) {}

    // TODO: distruttore, costruttori di copia, ecc.
    ~EvaluationVisitor() = default;

    void visitProgram(Program* progrNode) override;
    void visitStmtBlock(StmtBlock* stmtblockNode) override;
    void visitBlock(Block* blockNode) override;
    
    // Statement
    void visitPrintStmt(PrintStmt* statement) override;
    void visitSetStmt(SetStmt* statement) override;
    void visitInputStmt(InputStmt* statement) override;
    void visitWhileStmt(WhileStmt* statement) override;
    void visitIfStmt(IfStmt* statement) override;

    // NumExpr
    void visitOperator(Operator* opNode) override;
    void visitNumber(Number* numNode) override;
    void visitVariable(Variable* varNode) override;
    
    // BoolExpr
    void visitBoolOp(BoolOp* boolNode) override;
    void visitRelOp(RelOp* relNode) override;
    void visitBoolConst(BoolConst* boolconst) override;

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
