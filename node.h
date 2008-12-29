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
    int nbChildren();
    Node *childNode(int row);
    Node *parent();
    int indexOfChild(Node* child);

protected:
    Node *parent_;
    QList<Node *> children;
};

#endif // NODE_H
