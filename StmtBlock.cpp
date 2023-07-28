#include "StmtBlock.h"
#include "Statement.h"
#include "Block.h"
#include "Visitor.h"

void StmtBlock::accept(Visitor* v) {
    v->visitStmtBlock(this);
}

Statement* StmtBlock::getStatement() const {
   return statement;
}
Block* StmtBlock::getBlock() const {
   return block;
}