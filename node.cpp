#include "node.h"

Node::Node() : parent(0)
{
}

Node::~Node()
{
    qDeleteAll(children);
}

QVariant Node::data(int /*column*/, int /*role*/)
{
/*    if (role != Qt::DisplayRole)
        return QVariant();
    switch (column)
    {
    case 0:
        return int(this);
    case 1:
        return int(this)%97;
    }*/
    return QVariant();

}

void Node::addChild(Node *child)
{
    children.append(child);
    child->parent=this;
}
