#include <QtCore>
#include <QPixmap>

#include "model.h"
#include "ftp.h"
#include "domitem.h"

#include <QtDebug>

Model::Model(QObject *parent) : QAbstractItemModel(parent), domDocument(QDomDocument("ftp_index"))
{
    QDomElement domRoot = domDocument.createElement("ftp_list");
    rootItem = new DomItem(domRoot, 0);
    domDocument.appendChild(domRoot);
    connect(qApp, SIGNAL(aboutToQuit()),
            this, SLOT(save()));
}

Model::~Model()
{
    delete rootItem;
}

void Model::addFtp(QString &host)
{
    int pos = list.size();
    beginInsertRows(QModelIndex(), pos, pos);
    Ftp *ftp = new Ftp(host, rootItem);
    list.append(ftp);
    endInsertRows();
    connect(ftp,SIGNAL(modified(DomItem*)),
            this, SLOT(itemUpdated(DomItem*)));
    connect(ftp, SIGNAL(beginNewChild(DomItem*)),
            this, SLOT(beginNewChild(DomItem*)));
    connect(ftp, SIGNAL(endNewChild()),
            this, SLOT(endNewChild()));
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int Model::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}
QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::TextAlignmentRole && index.column() == 1)
        return Qt::AlignRight;

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole && role != Qt::DecorationRole)
        return QVariant();

    DomItem *item = static_cast<DomItem*>(index.internalPointer());

    QDomNode node = item->node();
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (index.column())
        {
        case 0:
            return node.attributes().namedItem("name").nodeValue();
        case 1:
            return humanReadableSize(recursiveSize(node));
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        if (index.column() == 0)
        {
            if (node.nodeName()=="ftp")
                return QPixmap(":/icons/computer.png");
            if (node.nodeName()=="dir")
                return QPixmap(":/icons/directory.png");
        }
    }
    return QVariant();
}

int Model::columnCount(const QModelIndex & /* parent */) const
{
    return 2;
}

QVariant Model::headerData(int section,  Qt::Orientation orientation, int role) const
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

void Model::save()
{
    const QString dir = QDir::homePath()+"/.burgos/";
    QDir d;
    if (!d.exists(dir))
        d.mkdir(dir);
    QFile file(dir + "index.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    domDocument.save(out, 4);
    file.close();
}

void Model::itemUpdated(DomItem *item)
{
    emit dataChanged(createIndex(item->row(),0,item->parent()), createIndex(item->row(),1,item->parent()));
}

void Model::beginNewChild(DomItem* item)
{
    int row = item->node().childNodes().count(); //Emplacement de la future ligne
    //beginInsertRows(createIndex(item->row(),0,item->parent()), row, row);
    //Je suis pas sur ce cette ligne, mais ça marche mieux comme ça.
    beginInsertRows(createIndex(0,0,item), row, row);
}

void Model::endNewChild()
{
    endInsertRows();
}

quint64 Model::recursiveSize(QDomNode node)
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


QString Model::humanReadableSize(qint64 intSize)
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
