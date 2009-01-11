#include <QtCore>
#include <QPixmap>

#include "ftpmodel.h"
#include "domitem.h"

FtpModel::FtpModel(QObject *parent) : QAbstractItemModel(parent)
{
    connect(&ftpHandler,SIGNAL(itemUpdated(DomItem*)),
            this, SLOT(itemUpdated(DomItem*)));
}

QModelIndex FtpModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = ftpHandler.root();
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FtpModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == ftpHandler.root())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FtpModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = ftpHandler.root();
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

QVariant FtpModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole && index.column() == 1)
        return Qt::AlignRight;

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole && role != Qt::DecorationRole)
        return QVariant();

    DomItem *item = static_cast<DomItem*>(index.internalPointer());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (index.column())
        {
        case 0:
            return item->node().attributes().namedItem("name").nodeValue();
        case 1:
            return humanReadableSize(recursiveSize(item->node()));
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        if (index.column() == 0)
        {
            QString name = item->node().nodeName();
            if (name=="ftp")
                return QPixmap(":/icons/computer.png");
            if (name=="dir")
                return QPixmap(":/icons/directory.png");
        }
    }
    return QVariant();
}

int FtpModel::columnCount(const QModelIndex & /* parent */) const
{
    return 2;
}

QVariant FtpModel::headerData(int section,  Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section)
        {
        case 0:
            return tr("Name");
        case 1:
            return tr("Size");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void FtpModel::addFtp(QString &host)
{
    int pos = ftpHandler.size();
    beginInsertRows(QModelIndex(), pos, pos);
    ftpHandler.addFtp(host);
    endInsertRows();
}

void FtpModel::itemUpdated(DomItem *item)
{
    emit dataChanged(createIndex(item->row(),0,item->parent()), createIndex(item->row(),1,item->parent()));
}

quint64 FtpModel::recursiveSize(QDomNode node)
{
    qint64 total=node.attributes().namedItem("size").nodeValue().toULongLong();

    QDomNode child = node.firstChild();
    while (!child.isNull())
    {
        total += recursiveSize(child);
        child = child.nextSibling();
    }
    return total;
}


QString FtpModel::humanReadableSize(qint64 intSize)
{
    static const QStringList prefix= QStringList() << "" << "K" << "M" << "G" << "T" << "P" << "E" << "Z" << "Y";
    double size=intSize;
    int exponent=0;
    while (size>1024)
    {
        size /= 1024;
        exponent +=1;
    }
    if (exponent > 8)
        return tr("Forty two");

    return QString::number(size, 'f', 1) + " " + prefix.at(exponent) + "o";
}
