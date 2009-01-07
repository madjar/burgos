//Code issu de "Simple DOM Model Example"

#include <QtXml>

#include "domitem.h"

DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
{
    domNode = node;
    // Record the item's location within its parent.
    rowNumber = row;
    parentItem = parent;
}

DomItem::~DomItem()
{
    QHash<int,DomItem*>::iterator it;
    for (it = childItems.begin(); it != childItems.end(); ++it)
        delete it.value();
}

QDomNode DomItem::node() const
{
    return domNode;
}

DomItem *DomItem::parent()
{
    return parentItem;
}

DomItem *DomItem::child(int i)
{
    if (childItems.contains(i))
        return childItems[i];

    if (i >= 0 && i < domNode.childNodes().count()) {
        QDomNode childNode = domNode.childNodes().item(i);
        DomItem *childItem = new DomItem(childNode, i, this);
        childItems[i] = childItem;
        return childItem;
    }
    return 0;
}

int DomItem::row()
{
    return rowNumber;
}

//Ça, c'est de moi (ça se voit)
QDomElement DomItem::element() const
{
    return domNode.toElement();
}

DomItem *DomItem::newChild(QString tagName)
{
    QDomNode newNode = domNode.ownerDocument().createElement(tagName);
    int i = domNode.childNodes().count(); //Position à laquelle on ajoute le QDomNode, ie sa ligne.
    domNode.appendChild(newNode);
    DomItem *newItem = new DomItem(newNode, i, this);
    childItems[i] = newItem;
    return newItem;
}
