#include "Block.h"
#include "Statement.h"
#include "Visitor.h"

void Block::addStatement(Statement* statement) {
    statements.push_back(statement);
}
std::vector<Statement*> Block::getStatements() const{
    return statements;
}

void Block::accept(Visitor* v) {
    v->visitBlock(this);
}
