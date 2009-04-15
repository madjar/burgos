/*

Copyright 2008 by Thomas Duboucher and Georges Dubus.

thomas.duboucher@supelec.fr
georges.dubus@supelec.fr

This software is a computer program whose purpose is to scan and
index ftp servers.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.

*/

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

DomItem *DomItem::newChild(const QString &tagName)
{
    QDomNode newNode = domNode.ownerDocument().createElement(tagName);
    int i = domNode.childNodes().count(); //Position à laquelle on ajoute le QDomNode, ie sa ligne.
    domNode.appendChild(newNode);
    DomItem *newItem = new DomItem(newNode, i, this);
    childItems[i] = newItem;
    return newItem;
}
