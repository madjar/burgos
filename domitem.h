#ifndef DOMITEM_H
#define DOMITEM_H

#include <QDomNode>
#include <QHash>
#include <QString>

class DomItem
{
public:
    DomItem(QDomNode &node, int row, DomItem *parent = 0);
    ~DomItem();
    DomItem *child(int i);
    DomItem *parent();
    QDomNode node() const;
    int row();

    QDomElement element() const;
    DomItem *newChild(QString tagName);

private:
    QDomNode domNode;
    QHash<int,DomItem*> childItems;
    DomItem *parentItem;
    int rowNumber;
};

#endif
