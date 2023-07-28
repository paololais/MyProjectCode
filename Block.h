#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

#include "Node.h"

//forward declaration di Statement
class Statement;

class Block : public Node {
public:
    Block(const std::vector<Statement*>& s) : statements(s) { }
    ~Block() = default;
    
    void accept(Visitor* v) override;

    // Aggiungi uno statement al blocco
    void addStatement(Statement* statement);

    std::vector<Statement*> getStatements() const;

private:
    std::vector<Statement*> statements;
};

#endif
