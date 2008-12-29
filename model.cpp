#include <QtCore>

#include "model.h"
#include "ftp.h"

#include <QtDebug>

Model::Model(QObject *parent) : QAbstractItemModel(parent), document(QDomDocument("ftp index"))
{
}

Model::~Model()
{
}

/*void Model::setRootNode(QDomNode node)
{
    rootNode = node;
    reset();
}*/

void Model::addFtp(QString &host)
{
    int pos = list.size();
    beginInsertRows(QModelIndex(), pos, pos);
    Ftp *ftp = new Ftp(host, document);
    list.append(ftp);
    connect(ftp,SIGNAL(modified(QDomNode)),
            this, SLOT(nodeUpdated(QDomNode)));
    endInsertRows();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    /*if (!rootNode || row < 0 || column < 0)
        return QModelIndex();
    Node *parentNode = nodeFromIndex(parent);
    Node *childNode = parentNode->childNode(row);
    if (!childNode)
        return QModelIndex();
    return createIndex(row, column, childNode);*/
    return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &child) const
{
    /*Node *node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();
    Node *parentNode = node->parent();
    if (!parentNode)
        return QModelIndex();
    Node *grandparentNode = parentNode->parent();
    if (!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->indexOfChild(parentNode);
    return createIndex(row, 0, parentNode);*/
    return QModelIndex();
}

int Model::rowCount(const QModelIndex &parent) const
{
    /*if (parent.column() > 0)
        return 0;
    Node *parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return 0;
    return parentNode->nbChildren();*/
    return 0;
}

int Model::columnCount(const QModelIndex & /* parent */) const
{
    return 2; //TODO
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    /*Node *node = nodeFromIndex(index);
    if (!node)
        return QVariant();

    return node->data(index.column(), role);*/
    return QVariant();
}

QVariant Model::headerData(int section,  Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Name");
        } else if (section == 1) {
            return tr("Size");
        } //TODO
    }
    return QVariant();
}

void Model::nodeUpdated(QDomNode node)
{
    //emit dataChanged(indexFromNode(node, 0), indexFromNode(node, 1));
}

QDomNode Model::nodeFromIndex(const QModelIndex &index) const
{
    /*if (index.isValid()) {
        return static_cast<Node *>(index.internalPointer());
    } else {
        return rootNode;
    }*/
    return QDomNode();
}

QModelIndex Model::indexFromNode(QDomNode node, int column)
{
    /*if (!node)
        return QModelIndex();
    Node *parentNode = node->parent();
    if (!parentNode)
        return QModelIndex();

    int row = parentNode->indexOfChild(node);
    return createIndex(row, column, parentNode);*/
    return QModelIndex();
}
