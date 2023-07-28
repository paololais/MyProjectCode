#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <variant>
#include <iostream>

#include "Program.h"
#include "StmtBlock.h"
#include "Statement.h"
#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"

// Visitor astratto per la visita degli oggetti del Program
class Visitor {
public:
    virtual void visitProgram(Program* progrNode) = 0;
    virtual void visitStmtBlock(StmtBlock* stmtblockNode) = 0;
    virtual void visitBlock(Block* blockNode) = 0;
    
    // Statement
    virtual void visitPrintStmt(PrintStmt* statement) = 0;
    virtual void visitSetStmt(SetStmt* statement) = 0;
    virtual void visitInputStmt(InputStmt* statement) = 0;
    virtual void visitWhileStmt(WhileStmt* statement) = 0;
    virtual void visitIfStmt(IfStmt* statement) = 0;

    // NumExpr
    virtual void visitOperator(Operator* opNode) = 0;
    virtual void visitNumber(Number* numNode) = 0;
    virtual void visitVariable(Variable* varNode) = 0;

    // BoolExpr
    virtual void visitBoolOp(BoolOp* boolNode) = 0;
    virtual void visitRelOp(RelOp* relNode) = 0;
    virtual void visitBoolConst(BoolConst* boolconst) = 0;
};

#endif