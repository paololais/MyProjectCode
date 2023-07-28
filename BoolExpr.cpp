#include "BoolExpr.h"
#include "Visitor.h"

void BoolOp::accept(Visitor* v){
    v->visitBoolOp(this);
}

void RelOp::accept(Visitor* v){
    v->visitRelOp(this);
}

void BoolConst::accept(Visitor* v){
    v->visitBoolConst(this);
}