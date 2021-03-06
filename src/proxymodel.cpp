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

#include "proxymodel.h"
#include "ftpmodel.h"
#include "domitem.h"
#include "def.h"

#include <QtXml>
#include <QSortFilterProxyModel>

ProxyModel::ProxyModel(QObject *parent) :
        QSortFilterProxyModel (parent)
{
    setSortCaseSensitivity ( Qt::CaseInsensitive );
    setSortRole(Burgos::SortRole);
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

    //Hack moche pour avoir un vrai classement décroissant sur la taille.
    if (left.column()==1)
        return !QSortFilterProxyModel::lessThan(left, right);

    return QSortFilterProxyModel::lessThan(left, right);
}


bool ProxyModel::filterAcceptsRow (int sourceRow,const QModelIndex & sourceParent) const
{
    if (filterRegExp().isEmpty())
        return true; // Inutile de faire une recherche récursive si on ne recherche pas.

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    //Looking if it's cached
    DomItem *item = static_cast<DomItem *>(index.internalPointer());
    if (item->cachedSearchName == filterRegExp().pattern())
    {
        return item->cachedSearchResult;
    }

    bool result = matches(index);

    // Saving to cache
    item->cachedSearchName = filterRegExp().pattern();
    item->cachedSearchResult = result;

    return result;
}

bool ProxyModel::matches(const QModelIndex &index) const
{
    // true if the name matches
    if (sourceModel()-> data(index).toString().contains(filterRegExp()))
        return true;

    // true of the name of one parent matches
    if (index.parent().isValid() && isSonOfExactMatchCached(index.parent()))
        return true;

    // true if one son matches (needed to dispay folders)
    for (int i=0; index.child(i, 0).isValid(); i++)
    {
        if (filterAcceptsRow(i, index))
            return true;
    }
    return false;
}

bool ProxyModel::isSonOfExactMatchCached(const QModelIndex &index) const
{
    DomItem *item = static_cast<DomItem *>(index.internalPointer());
    if (item->cachedSearchDirName == filterRegExp().pattern())
    {
        return item->cachedSearchDirResult;
    }

    bool result = isSonOfExactMatch(index);

    item->cachedSearchDirName = filterRegExp().pattern();
    item->cachedSearchDirResult = result;

    return result;
}

bool ProxyModel::isSonOfExactMatch(const QModelIndex &index) const
{
    if (sourceModel()-> data(index).toString().contains(filterRegExp()))
        return true;

    return index.parent().isValid() && isSonOfExactMatchCached(index.parent());
}
