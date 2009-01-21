#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QObject>

class ProxyModel : public QSortFilterProxyModel
{
public:
    ProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const;
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex & sourceParent
                          ) const ;
};

#endif // PROXYMODEL_H
