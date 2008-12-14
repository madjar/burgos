#include <QtCore>

#include "model.h"
#include "node.h"
#include "ftp.h"

#include <QtDebug>

Model::Model(QObject *parent) : QAbstractItemModel(parent)
{
    rootNode = new Node();
}

Model::~Model()
{
    delete rootNode;
}

void Model::setRootNode(Node *node)
{
    delete rootNode;
    rootNode = node;
    reset();
}

void Model::addFtp(QString &host)
{
    int pos = rootNode->children.size();
    beginInsertRows(QModelIndex(), pos, pos);
    Ftp *ftp = new Ftp(host);
    rootNode->addChild(ftp);
    connect(ftp,SIGNAL(modified(Node*)),
            this, SLOT(nodeUpdated(Node*)));
    endInsertRows();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if (!rootNode || row < 0 || column < 0)
        return QModelIndex();
    Node *parentNode = nodeFromIndex(parent);
    Node *childNode = parentNode->children.value(row);
    if (!childNode)
        return QModelIndex();
    return createIndex(row, column, childNode);
}

QModelIndex Model::parent(const QModelIndex &child) const
{
    Node *node = nodeFromIndex(child);
    if (!node)
        return QModelIndex();
    Node *parentNode = node->parent;
    if (!parentNode)
        return QModelIndex();
    Node *grandparentNode = parentNode->parent;
    if (!grandparentNode)
        return QModelIndex();

    int row = grandparentNode->children.indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    Node *parentNode = nodeFromIndex(parent);
    if (!parentNode)
        return 0;
    return parentNode->children.count();
}

int Model::columnCount(const QModelIndex & /* parent */) const
{
    return 2; //TODO
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    Node *node = nodeFromIndex(index);
    if (!node)
        return QVariant();

    return node->data(index.column(), role);
}

QVariant Model::headerData(int section,  Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return tr("Nom");
        } else if (section == 1) {
            return tr("Taille");
        } //TODO
    }
    return QVariant();
}

void Model::nodeUpdated(Node *node)
{
    emit dataChanged(indexFromNode(node, 0), indexFromNode(node, 1));
}

Node *Model::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        return static_cast<Node *>(index.internalPointer());
    } else {
        return rootNode;
    }
}

QModelIndex Model::indexFromNode(Node *node, int column)
{
    if (!node)
        return QModelIndex();
    Node *parentNode = node->parent;
    if (!parentNode)
        return QModelIndex();

    int row = parentNode->children.indexOf(node);
    return createIndex(row, column, parentNode);
}
