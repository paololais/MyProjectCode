#ifndef PRINT_VISITOR_H
#define PRINT_VISITOR_H

#include <vector>
#include <variant>
#include <iostream>

#include "Visitor.h"
#include "Statement.h"
#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"

// Visitor concreto per la stampa del programma
class PrintVisitor : public Visitor {
public:
    void visitProgram(Program* progrNode) override{}
    void visitStmtBlock(StmtBlock* stmtblockNode) override{}
    void visitBlock(Block* blockNode) override{}

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

};

#endif