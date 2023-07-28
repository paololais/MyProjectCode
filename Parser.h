#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "Token.h"
#include "NodeManager.h"
#include "Tokenizer.h"
#include "NumExpr.h"
#include "StmtBlock.h"
#include "BoolExpr.h"
#include "Exceptions.h"
#include "Block.h"
#include "Program.h"
#include "Statement.h"
#include "Node.h"

// Function object per il parsing di espressioni
// Funzione di parsing: restituisce "true" se l'espressione � corretta
// "false" altrimenti. Suppone che nella stringa vi siano solo simboli
// terminali per la grammatica definita sopra.
class ParseProgram {

public:
    ParseProgram(NodeManager& manager) : nm{ manager } { }

    Program* operator()(const std::vector<Token>& tokenStream);

    void printSyntaxTree(Node* node, int depth);

private:
    std::vector<Token>::const_iterator streamEnd;

    // Riferimento all'expression manager "di sistema"
    NodeManager& nm;

    // Parser del programma
    Program* parseProgram(std::vector<Token>::const_iterator& tokenItr);
    // Avanzamento "sicuro" di un iteratore
    void safe_next(std::vector<Token>::const_iterator& itr);

    StmtBlock* parseStmtBlock(std::vector<Token>::const_iterator& tokenItr);
    Block* parseBlock(std::vector<Token>::const_iterator& tokenItr);
    Statement* parseStatement(std::vector<Token>::const_iterator& tokenItr);
    SetStmt* parseSetStatement(std::vector<Token>::const_iterator& tokenItr);
    PrintStmt* parsePrintStatement(std::vector<Token>::const_iterator& tokenItr);
    InputStmt* parseInputStatement(std::vector<Token>::const_iterator& tokenItr);
    IfStmt* parseIfStatement(std::vector<Token>::const_iterator& tokenItr);
    WhileStmt* parseWhileStatement(std::vector<Token>::const_iterator& tokenItr);

    Variable* parseVariable(std::vector<Token>::const_iterator& itr);
    NumExpr* parseNumExpr(std::vector<Token>::const_iterator& itr);
    BoolExpr* parseBoolExpr(std::vector<Token>::const_iterator& itr);
};

#endif