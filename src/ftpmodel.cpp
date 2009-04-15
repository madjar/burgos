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

#include <QtCore>
#include <QPixmap>

#include "ftpmodel.h"
#include "domitem.h"
#include "def.h"

FtpModel::FtpModel(QObject *parent) : QAbstractItemModel(parent)
{
    connect(&ftpHandler, SIGNAL(itemUpdated(DomItem*)),
            this, SLOT(itemUpdated(DomItem*)));
    connect (&ftpHandler, SIGNAL(beginAddFtp(int)),
             this, SLOT(beginAddFtp(int)));
    connect (&ftpHandler, SIGNAL(endAddFtp()),
             this, SLOT(endAddFtp()));
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
    case Burgos::SortRole:
        switch (index.column())
        {
        case 0:
            return item->node().attributes().namedItem("name").nodeValue();
        case 1:
            return recursiveSize(item->node());
        default:
            return QVariant();
        }
    case Burgos::UrlRole:
        {
            return data(index.parent(), Burgos::UrlRole).toString() + QString("/")
                    + item->node().attributes().namedItem("name").nodeValue();
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

void FtpModel::addFtp(const QString &host)
{
    ftpHandler.addFtp(host);
}

void FtpModel::beginAddFtp(int pos)
{
    beginInsertRows(QModelIndex(), pos, pos);
}

void FtpModel::endAddFtp()
{
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
