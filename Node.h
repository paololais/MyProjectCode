#ifndef NODE_H
#define NODE_H

class Visitor;

class Node {
public:
    virtual ~Node() = default;

    virtual void accept(Visitor* v) = 0;
};

#endif