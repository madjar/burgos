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
            return tr("Pair", "peer hostname or ip address");
        }
        else if (section == 1)
        {
            return tr("Informations", "peer informations");
        }
    }
    return QVariant();
}

void PeerModel::update()
{
    QList<QVariant> rootData;
    if (this->rootItem->rowCount() > 1)
    {
       rootData << "" << tr("%n pair(s) disponible(s)", "other users are found", this->rootItem->rowCount()-1);
       this->rootItem->set(rootData);
    }
    else
    {
       rootData << "" << tr("Aucun pairs disponibles", "no other users are found");
       this->rootItem->set(rootData);
    }
    emit changed(QModelIndex());
}
