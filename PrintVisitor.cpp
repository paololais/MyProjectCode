#include "PrintVisitor.h"

void PrintVisitor::visitNumber(Number* numNode){
    std::cout << numNode->get_Value();
}
void PrintVisitor::visitVariable(Variable* varNode){
    std::cout << varNode->get_value();
}
void PrintVisitor::visitBoolConst(BoolConst* boolconst){
    std::cout << (boolconst->getBoolConst() ? "TRUE" : "FALSE");
}

void PrintVisitor::visitOperator(Operator* opNode) {
    std::cout << "( ";
    switch (opNode->getOp()) {
        case Operator::ADD:
            std::cout << "ADD ";
            break;
        case Operator::SUB:
            std::cout << "SUB ";
            break;
        case Operator::MUL:
            std::cout << "MUL ";
            break;
        case Operator::DIV:
            std::cout << "DIV ";
            break;
        default:
            break;
    }
    // Visit degli operandi
    opNode->getLeft()->accept(this);
    opNode->getRight()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitRelOp(RelOp* relNode) {
    std::cout << "( ";
    switch (relNode->getOp()) {
        case RelOp::LT:
            std::cout << "LT ";
            break;
        case RelOp::GT:
            std::cout << "GT ";
            break;
        case RelOp::EQ:
            std::cout << "EQ ";
            break;
        default:
            break;
    }
    // Visit degli operandi
    relNode->getLeft()->accept(this);
    relNode->getRight()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitBoolOp(BoolOp* boolNode) {
    std::cout << "( ";
    switch (boolNode->getOpCode()) {
        case BoolOp::AND:
            std::cout << "AND ";
            // Visit degli operandi
            boolNode->getBoolExpr1()->accept(this);
            boolNode->getBoolExpr2()->accept(this);
            break;
        case BoolOp::OR:
            std::cout << "OR ";
            // Visit degli operandi
            boolNode->getBoolExpr1()->accept(this);
            boolNode->getBoolExpr2()->accept(this);
            break;
        case BoolOp::NOT:
            std::cout << "NOT ";
            // Visit degli operandi
            boolNode->getBoolExpr1()->accept(this);
            break;
        default:
            break;
    }
    std::cout << ")";
}

void PrintVisitor::visitPrintStmt(PrintStmt* statement) {
    // Implementazione per la visita di PrintStmt
    std::cout << "(PRINT ";
    statement->getNumExpr()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitSetStmt(SetStmt* statement) {
    // Implementazione per la visita di SetStmt
    std::cout << "(SET ";
    statement->getVariable()->accept(this);
    std::cout << " ";
    statement->getNumExpr()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitInputStmt(InputStmt* statement) {
    // Implementazione per la visita di InputStmt
    std::cout << "(INPUT ";
    statement->getVariable()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitWhileStmt(WhileStmt* statement) {
    // Implementazione per la visita di WhileStmt
    std::cout << "(WHILE ";
    statement->getBoolExpr()->accept(this);
    std::cout << " ";
    statement->getStmtBlock()->accept(this);
    std::cout << ")";
}

void PrintVisitor::visitIfStmt(IfStmt* statement) {
    // Implementazione per la visita di IfStmt
    std::cout << "(IF ";
    statement->getBoolExpr()->accept(this);
    std::cout << " ";
    statement->getStmtBlock1()->accept(this);
    if (statement->getStmtBlock2()) {
        std::cout << " ELSE ";
        statement->getStmtBlock2()->accept(this);
    }
    std::cout << ")";
}