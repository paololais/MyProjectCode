#include "Parser.h"
#include "Token.h"
#include "StmtBlock.h"
#include "NodeManager.h"
#include "Tokenizer.h"
#include "NumExpr.h"
#include "BoolExpr.h"
#include "Exceptions.h"
#include "Block.h"
#include "Program.h"
#include "Statement.h"
#include "Node.h"

#include <sstream>
#include <iostream>

Program* ParseProgram::operator()(const std::vector<Token>& tokenStream) {
    auto tokenItr = tokenStream.begin();
    streamEnd = tokenStream.end();
    Program* progr = parseProgram(tokenItr);
    if (tokenItr != streamEnd) {
        throw ParseError("Unexpected tokens after parsing");
        return nullptr;
    }
    return progr;
}

void ParseProgram::safe_next(std::vector<Token>::const_iterator& itr) {
    if (itr == streamEnd) {
        throw ParseError("Unexpected end of input");
    }
    ++itr;
}

 Program* ParseProgram::parseProgram(std::vector<Token>::const_iterator& tokenItr){
    if (tokenItr == streamEnd) {
        throw ParseError("Empty Program");
    }
    StmtBlock* s = parseStmtBlock(tokenItr);
    return static_cast<Program*>(nm.makeProgram(s));
 }

StmtBlock* ParseProgram::parseStmtBlock(std::vector<Token>::const_iterator& itr) {

    // block o statement iniziano sempre con '(')
    if (itr->tag == Token::LP) {
        // 2 casi:
        // statement | ( BLOCK statement_list )
        safe_next(itr);
        if (itr->tag == Token::BLOCK)
        {
            // Caso di blocco
            Block* b = parseBlock(itr);
            return static_cast<StmtBlock*>(nm.makeStmtBlock(b));
        } else {
            Statement* s = parseStatement(itr);
            return static_cast<StmtBlock*>(nm.makeStmtBlock(s));
        }
    } else {
        throw ParseError("Expected token '(' before stmt_block.");
        return nullptr;
    }
}

Block* ParseProgram::parseBlock(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip Token::BLOCK

    // Inizializza la lista di istruzioni all'interno del blocco
    std::vector<Statement*> statement_list;

    // Analizza le istruzioni all'interno del blocco fino alla parentesi chiusa
    do {
        if (tokenItr->tag != Token::LP) {
            throw ParseError("Expected '(' before statement in BLOCK");
        }
        safe_next(tokenItr); // Skip Token::LP
         // Analizza una singola istruzione
        Statement* statement = parseStatement(tokenItr);
        // Aggiungi l'istruzione alla lista di istruzioni del blocco
        statement_list.push_back(statement);
    } while (tokenItr->tag != Token::RP);
    
    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after statement_list in BLOCK");
    }
    // Salta la parentesi chiusa ')'
    safe_next(tokenItr);

    // Crea e restituisci il blocco con la lista di istruzioni
    return static_cast<Block*>(nm.makeBlock(statement_list));

}

Statement* ParseProgram::parseStatement(std::vector<Token>::const_iterator& tokenItr) {
    if (tokenItr->tag == Token::SET) {
        return parseSetStatement(tokenItr);
    }
    else if (tokenItr->tag == Token::PRINT) {
        return parsePrintStatement(tokenItr);
    }
    else if (tokenItr->tag == Token::INPUT) {
        return parseInputStatement(tokenItr);
    }
    else if (tokenItr->tag == Token::IF) {
        return parseIfStatement(tokenItr);
    }
    else if (tokenItr->tag == Token::WHILE) {
        return parseWhileStatement(tokenItr);
    }
    else {
        throw ParseError("Unexpected KEYWORD statement");
    }
}

SetStmt* ParseProgram::parseSetStatement(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip SET
    if (tokenItr->tag != Token::VAR) {
        throw ParseError("Expected variable id after SET");
    }
    Variable* v = parseVariable(tokenItr);
    // next is num expr
    NumExpr* expr = parseNumExpr(tokenItr);
    // necessaria parentesi chiusa ')'
    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after expression in SET statement");
    }
    safe_next(tokenItr); // Skip RP
    return static_cast<SetStmt*>(nm.makeSetStmt(v, expr));
}

PrintStmt* ParseProgram::parsePrintStatement(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip PRINT
    NumExpr* expr = parseNumExpr(tokenItr);

    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after expression in PRINT");
    }
    safe_next(tokenItr); // Skip RP

    return static_cast<PrintStmt*>(nm.makePrintStmt(expr));
}

InputStmt* ParseProgram::parseInputStatement(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip INPUT

    if (tokenItr->tag != Token::VAR) {
        throw ParseError("Expected variable name after INPUT");
    }

    Variable* v = parseVariable(tokenItr);
    
    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after expression in INPUT");
    }
    safe_next(tokenItr); // Skip RP

    return static_cast<InputStmt*>(nm.makeInputStmt(v));
}

IfStmt* ParseProgram::parseIfStatement(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip IF

    BoolExpr* condition = parseBoolExpr(tokenItr);

    StmtBlock* sbTrue = parseStmtBlock(tokenItr);
    
    StmtBlock* sbFalse = parseStmtBlock(tokenItr);
    
    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after blocks in IF");
    }
    safe_next(tokenItr);
    return static_cast<IfStmt*>(nm.makeIfStmt(condition, sbTrue, sbFalse));
}

WhileStmt* ParseProgram::parseWhileStatement(std::vector<Token>::const_iterator& tokenItr) {
    safe_next(tokenItr); // Skip WHILE
    BoolExpr* condition = parseBoolExpr(tokenItr);

    StmtBlock* stmtBlock = parseStmtBlock(tokenItr);

    if (tokenItr->tag != Token::RP) {
        throw ParseError("Expected ')' after stmt_block in WHILE");
    }
    safe_next(tokenItr); // Skip RP

    return static_cast<WhileStmt*>(nm.makeWhileStmt(condition, stmtBlock));
}

Variable* ParseProgram::parseVariable(std::vector<Token>::const_iterator &itr){
    std::stringstream temp{};
    temp << itr->word;
    std::string variable_id;
    temp >> variable_id;
    safe_next(itr);
    return static_cast<Variable*>(nm.makeVariable(variable_id));
}

NumExpr* ParseProgram::parseNumExpr(std::vector<Token>::const_iterator& itr) {
    if (itr->tag == Token::LP) {
        // Devo analizzare un'espressione "composita": (operator E1 E2)
        safe_next(itr);
        Operator::OpCode op;
        switch (itr->tag) {
            case Token::ADD: op = Operator::ADD; break;
            case Token::SUB: op = Operator::SUB; break;
            case Token::MUL: op = Operator::MUL; break;
            case Token::DIV: op = Operator::DIV; break;
            default:
                throw ParseError("Unrecognized Num operator");
        }
        // avanzo a E1
        safe_next(itr);
        NumExpr* left = parseNumExpr(itr);
        // E2
        NumExpr* right = parseNumExpr(itr);
        // Controllo che alla posizione corrente ci sia una ')'
        if (itr->tag != Token::RP) {
            throw ParseError("Mismatched parenthesis");
        }
        safe_next(itr);
        return static_cast<NumExpr*>(nm.makeOperator(op, left, right));
    } else if (itr->tag == Token::NUM) {
        std::stringstream temp{};
        temp << itr->word;
        int value;
        temp >> value;
        safe_next(itr);
        return static_cast<NumExpr*>(nm.makeNumber(value));
    } else if(itr->tag == Token::VAR){
        return static_cast<NumExpr*>(parseVariable(itr));        
    } else {
        throw ParseError("Unexpected token in NumExpr");
    }
}

BoolExpr* ParseProgram::parseBoolExpr(std::vector<Token>::const_iterator& itr) {
    if (itr->tag == Token::LP) {
        safe_next(itr);
        // Devo analizzare un'espressione "composita": (operator E1 E2)
        // 2 casi: una RelOp o una BoolOp
        //caso boolop
        if (itr->tag == Token::NOT || itr->tag == Token::AND || itr->tag == Token::OR)
        {
            bool isNot = false;
            BoolOp::OpCode op;
            switch (itr->tag) {
                case Token::NOT: 
                    op = BoolOp::NOT; 
                    isNot = true; 
                    break;
                case Token::AND: op = BoolOp::AND; break;
                case Token::OR: op = BoolOp::OR; break;
                default:
                    throw ParseError("Unrecognized BoolOp operator");
            }

            safe_next(itr);
            if (isNot) {
                BoolExpr* bexpr = parseBoolExpr(itr);
                // Controllo che alla posizione corrente ci sia una ')'
                if (itr->tag != Token::RP) {
                    throw ParseError("Mismatched parenthesis after 'NOT boolexpr'");
                }
                safe_next(itr);
                return static_cast<BoolExpr*>(nm.makeBoolOpNot(op, bexpr));
            } else{
                // E1
                BoolExpr* left = parseBoolExpr(itr);
                // E2
                BoolExpr* right = parseBoolExpr(itr);
                // Controllo che alla posizione corrente ci sia una ')'
                if (itr->tag != Token::RP) {
                    throw ParseError("Mismatched parenthesis after BoolOp'");
                }
                safe_next(itr);
                return static_cast<BoolExpr*>(nm.makeBoolOp(op, left, right));
            }
        }
        //caso relop
        else if (itr->tag == Token::LT || itr->tag == Token::GT || itr->tag == Token::EQ){
            RelOp::OpCode op;
            switch (itr->tag){
                case Token::LT: op = RelOp::LT; break;
                case Token::GT: op = RelOp::GT; break;
                case Token::EQ: op = RelOp::EQ; break;
                default: throw ParseError("Unrecognized RelOp operator");
            }
            safe_next(itr);
            // E1
            NumExpr* left = parseNumExpr(itr);
            // E2
            NumExpr* right = parseNumExpr(itr);
            // Controllo che alla posizione corrente ci sia una ')'
            if (itr->tag != Token::RP) {
                throw ParseError("Mismatched parenthesis after RelOp");
            }
            safe_next(itr);
            return static_cast<BoolExpr*>(nm.makeRelOp(op, left, right));
        } else {
            throw ParseError("Unrecognized Bool operator");
            return nullptr;
        }
    } else if (itr->tag == Token::FALSE || itr->tag == Token::TRUE) {
        bool op;
        switch (itr->tag){
            case Token::FALSE: op = false; break;
            case Token::TRUE: op = true; break;
            default: throw ParseError("Unrecognized boolconst");
        }
        safe_next(itr);
        return static_cast<BoolExpr*>(nm.makeBoolConst(op));
    } else {
        throw ParseError("Unexpected token parseBoolExpr");
        return nullptr; 
    }
}
// Funzione per la stampa dell'AST
void ParseProgram::printSyntaxTree(Node* node, int depth) {
    if (node == nullptr) {
        return;
    }

    // Aggiungi una serie di spazi proporzionale alla profondità del nodo
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    // Stampa il tipo del nodo e le sue informazioni specifiche (es. valore, operatore, etc.)
    if (Number* numNode = dynamic_cast<Number*>(node)) {
        std::cout << "Number: " << numNode->get_Value() << std::endl;
    } else if (Operator* opNode = dynamic_cast<Operator*>(node)) {
        std::cout << "Operator: " << opNode->getOp() << std::endl;
        printSyntaxTree(opNode->getLeft(), depth+1); 
        printSyntaxTree(opNode->getRight(), depth+1);
    } else if (Variable* varNode = dynamic_cast<Variable*>(node)) {
        std::cout << "Variable: " << varNode->get_variable_id() << std::endl;
    } else if (BoolOp* boolOpNode = dynamic_cast<BoolOp*>(node)) {
        std::cout << "BoolOp: " << boolOpNode->getOpCode() << std::endl;
        printSyntaxTree(boolOpNode->getBoolExpr1(), depth+1); 
        if (boolOpNode->getOpCode() != BoolOp::NOT)
        {
            printSyntaxTree(boolOpNode->getBoolExpr2(), depth+1);
        }
    } else if (RelOp* relOpNode = dynamic_cast<RelOp*>(node)) {
        std::cout << "RelOp: " << relOpNode->getOp() << std::endl;
        printSyntaxTree(relOpNode->getLeft(), depth+1); 
        printSyntaxTree(relOpNode->getRight(), depth+1);
    } else if (BoolConst* boolConstNode = dynamic_cast<BoolConst*>(node)) {
        std::cout << "BoolConst: " << boolConstNode->getBoolConst() << std::endl;
    } else if (Program* progrNode = dynamic_cast<Program*>(node)) {
        std::cout << "Program:" << std::endl;
        printSyntaxTree(progrNode->getStmtBlock(), depth + 1);
    } else if (StmtBlock* stmtBlockNode = dynamic_cast<StmtBlock*>(node)) {
        std::cout << "StmtBlock:" << std::endl;
        if (stmtBlockNode->getStatement() != nullptr) {
            printSyntaxTree(stmtBlockNode->getStatement(), depth + 1);
        } else if (stmtBlockNode->getBlock() != nullptr) {
            printSyntaxTree(stmtBlockNode->getBlock(), depth + 1);
        }
    } else if (Block* blockNode = dynamic_cast<Block*>(node)) {
        std::cout << "Block:" << std::endl;
        for (Statement* stmt : blockNode->getStatements()) {
            printSyntaxTree(stmt, depth + 1);
        }
    } else if (auto* printStmtNode = dynamic_cast<PrintStmt*>(node)) {
        std::cout << "PrintStmt" << std::endl;
        printSyntaxTree(printStmtNode->getNumExpr(), depth + 1);
    } else if (auto* setStmtNode = dynamic_cast<SetStmt*>(node)) {
        std::cout << "SetStmt" << std::endl;
        printSyntaxTree(setStmtNode->getVariable(), depth + 1);
        printSyntaxTree(setStmtNode->getNumExpr(), depth + 1);
    } else if (auto* inputStmtNode = dynamic_cast<InputStmt*>(node)) {
        std::cout << "InputStmt" << std::endl;
        printSyntaxTree(inputStmtNode->getVariable(), depth + 1);
    } else if (auto* whileStmtNode = dynamic_cast<WhileStmt*>(node)) {
        std::cout << "WhileStmt" << std::endl;
        printSyntaxTree(whileStmtNode->getBoolExpr(), depth + 1);
        printSyntaxTree(whileStmtNode->getStmtBlock(), depth + 1);
    } else if (auto* ifStmtNode = dynamic_cast<IfStmt*>(node)) {
        std::cout << "IfStmt" << std::endl;
        printSyntaxTree(ifStmtNode->getBoolExpr(), depth + 1); 
        printSyntaxTree(ifStmtNode->getStmtBlock1(), depth + 1);
        printSyntaxTree(ifStmtNode->getStmtBlock2(), depth + 1);
    } else {
        // Aggiungi un caso per ogni tipo di nodo dell'AST che hai definito
        // oppure un caso di default per gestire eventuali nuovi tipi in futuro
        std::cout << "Unknown node type" << std::endl;
    }
}
