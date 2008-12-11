#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QVariant>

class Node
{
public:
    Node();
    virtual ~Node();

    virtual QVariant data(int column, int role);
    void addChild(Node *child);

    Node *parent;
    QList<Node *> children;
};

#endif // NODE_H
