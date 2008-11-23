#include "proxymodel.h"
#include "file.h"
#include "model.h"

#include <QSortFilterProxyModel>

#include <QtDebug>

ProxyModel::ProxyModel(QObject *parent) :
        QSortFilterProxyModel (parent)
{
    setSortCaseSensitivity ( Qt::CaseInsensitive );
}

bool ProxyModel::lessThan(const QModelIndex &left,
                          const QModelIndex &right) const
{

    File *leftFile =
            static_cast<File *>(left.internalPointer());
    File *rightFile =
            static_cast<File *>(right.internalPointer());

    if (leftFile && rightFile && leftFile->isDir()!=rightFile->isDir())
        return leftFile->isDir();

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
