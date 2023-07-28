#ifndef PROGRAM_H
#define PROGRAM_H

#include "Node.h"
#include "StmtBlock.h"

class Program : public Node{
public:
    Program(StmtBlock* b) : stmt_block(b) { }

    ~Program() = default;
    
    StmtBlock* getStmtBlock(){
        return stmt_block;
    }

    void accept(Visitor* v) override;

private:
    StmtBlock* stmt_block;
};

#endif
