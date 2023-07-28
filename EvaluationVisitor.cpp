#include "EvaluationVisitor.h"
#include <algorithm>

void EvaluationVisitor::visitProgram(Program* progrNode){
    StmtBlock* stmtBlock = progrNode->getStmtBlock();
    stmtBlock->accept(this);
}

void EvaluationVisitor::visitStmtBlock(StmtBlock* stmtblockNode) {
     // Controlla se StmtBlock contiene un oggetto Statement
    if (stmtblockNode->getStatement() != nullptr) {
        Statement* stmt = stmtblockNode->getStatement();
        stmt->accept(this);
    }
    // Controlla se StmtBlock contiene un oggetto Block
    else if (stmtblockNode->getBlock() != nullptr) {
        Block* block = stmtblockNode->getBlock();
        block->accept(this);
    }
}

void EvaluationVisitor::visitBlock(Block* blockNode) {
    for (Statement* stmt : blockNode->getStatements()) {
        stmt->accept(this);
    }
}

void EvaluationVisitor::visitNumber(Number* numNode){
    // Il metodo push_back inserisce il valore come ultimo elemento del vector
    // (equivale ad una PUSH in uno stack)
    intAccumulator.push_back(numNode->get_Value());
}
void EvaluationVisitor::visitOperator(Operator* opNode) {
    // Valuta il valore dell'operando sinistro e destro
    opNode->getLeft()->accept(this);
    int leftValue = getBackIntAccumulator(); intAccumulator.pop_back();

    opNode->getRight()->accept(this);
    int rightValue = getBackIntAccumulator(); intAccumulator.pop_back();

    // Esegui l'operazione in base all'OpCode dell'operatore
    Operator::OpCode op = opNode->getOp();
    switch (op) {
        case Operator::ADD:
            intAccumulator.push_back(leftValue + rightValue);
            break;
        case Operator::SUB:
            intAccumulator.push_back(leftValue - rightValue);
            break;
        case Operator::MUL:
            intAccumulator.push_back(leftValue * rightValue);
            break;
        case Operator::DIV:
            if (rightValue == 0) {
                throw EvalError("Division by zero");
                return;
            }
            intAccumulator.push_back(leftValue / rightValue);
            break;
        default:
            break;
    }
}

void EvaluationVisitor::visitVariable(Variable* varNode){
    if (!varNode) {
        throw EvalError("Null pointer encountered in variable");
    }
    // Get the variable_id from the Variable node
    std::string variable_id = varNode->get_variable_id();

    // Check if the variable is already defined in the symbol table
    auto it = symbolTable.find(variable_id);
    //Se l'iteratore non punta alla fine della tabella dei simboli allora la variabile è stata trovata
    if (it != symbolTable.end()) {
        //se la variabile esiste push del valore nel intAccumulator
        intAccumulator.push_back(it->second);
    } else {
        // se la variabile non è nella symbol table allora è indefinita
        // a meno che non siamo nel contesto di una SET, lanciamo un'eccezione
        if (!isInSetContext) {
            throw EvalError("Use of undefined Variable: " + variable_id);
        } else {
            //siamo in una set->definizione di una nuova variabile, inserisco nella symbol table
            symbolTable[variable_id] = 0;
        }
    }
}

void EvaluationVisitor::visitBoolOp(BoolOp* boolNode) {
    if (!boolNode) {
        throw EvalError("Null pointer encountered in boolOp");
    }
    boolNode->getBoolExpr1()->accept(this);
    bool lval = getBackBoolAccumulator(); boolAccumulator.pop_back();
    
    BoolExpr* right = boolNode->getBoolExpr2();
    if (right != nullptr) {
        right->accept(this);
        bool rval = getBackBoolAccumulator(); boolAccumulator.pop_back();
        BoolOp::OpCode op = boolNode->getOpCode();
        switch (op) {
            case BoolOp::AND: boolAccumulator.push_back(lval && rval); break;;
            case BoolOp::OR: boolAccumulator.push_back(lval || rval); break;
            default: throw EvalError("Invalid BoolOp."); break;
        }
    } else{
        //caso NOT
        if(boolNode->getOpCode()==BoolOp::NOT){
            boolAccumulator.push_back(!lval); 
        } else {
            throw EvalError("Invalid BoolOp.");
        }
    }
}
void EvaluationVisitor::visitRelOp(RelOp* relNode){
    if (!relNode) {
        throw EvalError("Null pointer encountered in relOp");
    }
    // Prelevo i puntatori agli operandi e gli faccio accettare 
    // questo oggetto come visitatore (propago la visita)
    relNode->getLeft()->accept(this);
    int lval = getBackIntAccumulator(); intAccumulator.pop_back();

    relNode->getRight()->accept(this);
    int rval = getBackIntAccumulator(); intAccumulator.pop_back();

    // Valuto l'espressione relazionale in base all'operatore
    RelOp::OpCode op = relNode->getOp();
    switch (op) {
        case RelOp::LT:
            intAccumulator.push_back(lval < rval); break;;
        case RelOp::GT:
            intAccumulator.push_back(lval > rval); break;
        case RelOp::EQ:
            intAccumulator.push_back(lval == rval); break;;
        default:
           break;
    }
}
void EvaluationVisitor::visitBoolConst(BoolConst* boolconst){
    boolAccumulator.push_back(boolconst->getBoolConst());
}
// Statement
void EvaluationVisitor::visitPrintStmt(PrintStmt* statement) {
    statement->getNumExpr()->accept(this);
    std::cout << getBackIntAccumulator() << std::endl;
    intAccumulator.pop_back();
}
void EvaluationVisitor::visitSetStmt(SetStmt *statement){
    isInSetContext = true;
    statement->getNumExpr()->accept(this);
    int value = getBackIntAccumulator();
    intAccumulator.pop_back();
    // Aggiungi la variabile alla tabella dei simboli e imposta il valore
    std::string variable_id = statement->getVariable()->get_variable_id();
    symbolTable[variable_id] = value;
    isInSetContext = false;
}
void EvaluationVisitor::visitInputStmt(InputStmt *statement)
{
    int value;
    std::cout << "Enter an integer value: ";
    std::cin >> value;
    if (!std::cin.fail()) {
        // Aggiungi la variabile alla tabella dei simboli e imposta il valore
        std::string variable_id = statement->getVariable()->get_variable_id();
        symbolTable[variable_id] = value;
    }
    else {
        throw EvalError("Invalid input! Please enter an integer.");
    }
}
void EvaluationVisitor::visitWhileStmt(WhileStmt* statement) {
    while (true) {
        // Valuta la condizione del while
        statement->getBoolExpr()->accept(this);
        bool condValue = getBackBoolAccumulator();
        boolAccumulator.pop_back();
        if (!condValue) break;
        // se true esegui il corpo del while
        statement->getStmtBlock()->accept(this);
    }
}

void EvaluationVisitor::visitIfStmt(IfStmt* statement) {
    // Valuta la condizione dell'if
    statement->getBoolExpr()->accept(this);
    bool condValue = getBackBoolAccumulator();
    boolAccumulator.pop_back();
    if (condValue){
        // Esegui il blocco then dell'if true
        statement->getStmtBlock1()->accept(this);
    } else {
        // Esegui il blocco then dell'if false
        statement->getStmtBlock2()->accept(this);
    }
}
