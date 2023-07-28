#ifndef STMTBLOCK_H
#define STMTBLOCK_H

#include <vector>
#include <variant>

#include "Node.h"

class Statement;
class Block;

class StmtBlock : public Node {
public:
    StmtBlock(Statement* stmt) : statement(stmt), block(nullptr) {}
    StmtBlock(Block* b) : statement(nullptr), block(b) {}
    ~StmtBlock() = default;

    void accept(Visitor* v) override;

    Statement* getStatement() const;
    Block* getBlock() const;

private:
    Statement* statement;
    Block* block;
};

#endif
