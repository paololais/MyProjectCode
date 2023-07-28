#ifndef STATEMENT_H
#define STATEMENT_H

#include "Block.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "StmtBlock.h"

class Statement : public Node{
public:
    virtual ~Statement() {};

    virtual void accept(Visitor* v) = 0;
};

class PrintStmt : public Statement{
public:
    PrintStmt(NumExpr* n) : expr {n} { }

    NumExpr* getNumExpr(){
        return expr;
    }

    void accept(Visitor* v) override;
private:
    NumExpr* expr;
};

class SetStmt : public Statement{
public:
    SetStmt(NumExpr* n, Variable* v) : expr{n}, var{ v } { }
    void accept(Visitor* v) override;

    NumExpr* getNumExpr(){
        return expr;
    }
    Variable* getVariable(){
        return var;
    }

private:
    NumExpr* expr;   
    Variable* var;
};

class InputStmt : public Statement{
public:
    InputStmt(Variable* v) : var{ v } { }

    void accept(Visitor* v) override;

    Variable* getVariable(){
        return var;
    }
private:
    Variable* var;    
};

class WhileStmt : public Statement{
public:
    WhileStmt(BoolExpr* b, StmtBlock* blc) : bExpr{ b }, block{ blc }{ }

    void accept(Visitor* v) override;

    BoolExpr* getBoolExpr(){
        return bExpr;
    }
    StmtBlock* getStmtBlock(){
        return block;
    }
private:
    BoolExpr* bExpr;  
    StmtBlock* block;
};

class IfStmt : public Statement{
public:
    IfStmt(BoolExpr* b, StmtBlock* blc1, StmtBlock* blc2) : 
        expr {b}, block1{ blc1 }, block2 { blc2 } {}

    void accept(Visitor* v) override;

    BoolExpr* getBoolExpr(){
        return expr;
    }
    StmtBlock* getStmtBlock1(){
        return block1;
    }
    StmtBlock* getStmtBlock2(){
        return block2;
    }    
private:
    BoolExpr* expr; 
    StmtBlock* block1;
    StmtBlock* block2;   
};

#endif
