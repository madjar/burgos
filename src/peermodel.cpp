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

#include "peermodel.h"

#include <QString>
#include <QPixmap>

PeerModel::PeerModel(QObject *parent) : QAbstractItemModel(parent)
{
    this->rootItem = new Peer(QList<QVariant>());
    PeerModel::update();
}

PeerModel::~PeerModel()
{
    delete this->rootItem;
}

QModelIndex PeerModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    Peer *parentItem;
    if (!parent.isValid())
        parentItem = this->rootItem;
    else
        parentItem = static_cast<Peer*>(parent.internalPointer());
    Peer *childItem = parentItem->get(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PeerModel::parent(const QModelIndex &/*child*/) const
{
    return QModelIndex();
}

int PeerModel::rowCount(const QModelIndex &parent) const
{
    Peer *parentItem;
    if (parent.column() > 0) { return 0; }
    if (!parent.isValid())
    {
        parentItem = this->rootItem;
    }
    else
    {
        parentItem = static_cast<Peer*>(parent.internalPointer());
    }
    return parentItem->rowCount();
}

int PeerModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<Peer*>(parent.internalPointer())->columnCount();
    }
    else
    {
        return this->rootItem->columnCount();
    }
}

QVariant PeerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole && index.column() == 0)
        return QPixmap(":/icons/computer.png");
    if (!index.isValid())
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();
    Peer *item = static_cast<Peer*>(index.internalPointer());
    return item->data(index.column());
}

QVariant PeerModel::headerData(int section,  Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
        {
            //: peer hostname or ip address
            return tr("Peer");
        }
        else if (section == 1)
        {
            //: peer information
            return tr("Information");
        }
    }
    return QVariant();
}

void PeerModel::update()
{
    QList<QVariant> rootData;

    //: Number of others users found
    rootData << QString("") << tr("%n peer(s) available", "", this->rootItem->rowCount()-1);
    this->rootItem->set(rootData);

    emit changed(QModelIndex());
}
