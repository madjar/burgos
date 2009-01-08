#include "proxymodel.h"
#include "ftpmodel.h"
#include "domitem.h"

#include <QtXml>
#include <QSortFilterProxyModel>

ProxyModel::ProxyModel(QObject *parent) :
        QSortFilterProxyModel (parent)
{
    setSortCaseSensitivity ( Qt::CaseInsensitive );
}

bool ProxyModel::lessThan(const QModelIndex &left,
                          const QModelIndex &right) const
{
    DomItem *leftFile =
            static_cast<DomItem *>(left.internalPointer());
    DomItem *rightFile =
            static_cast<DomItem *>(right.internalPointer());

    if (leftFile && rightFile && (leftFile->node().nodeName()=="dir")!=(rightFile->node().nodeName()=="dir"))
    {
        return leftFile->node().nodeName()=="dir";
    }

    return QSortFilterProxyModel::lessThan(left, right);
}


bool ProxyModel::filterAcceptsRow (int sourceRow,const QModelIndex & sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    bool result = sourceModel()-> data(index).toString().contains(filterRegExp());
    for (int i=0; index.child(i, 0).isValid(); i++)
    {
        QModelIndex child = index.child(i, 0);
        result = result || filterAcceptsRow(i, index);
    }
    return result;
}
