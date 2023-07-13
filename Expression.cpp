#include "Expression.h"
#include "Visitor.h"

void Number::accept(Visitor* v) {
    v->visitNumber(this);
}

void Operator::accept(Visitor* v) {
    v->visitOperator(this);
}

