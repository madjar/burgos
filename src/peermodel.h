#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QVariant>
#include <QList>
#include <QAbstractItemModel>

#include "peer.h"

/*
 * PeerModel.h
 * QAbstractItemModel pour afficher les pairs sur la fenetre de burgos
 */
class PeerModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    PeerModel(QObject *parent = 0);
    ~PeerModel();

signals:
    void changed(const QModelIndex &);

public:
    QModelIndex index(int , int ,
                      const QModelIndex &) const;
    QModelIndex parent(const QModelIndex &) const;

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int ) const;
    QVariant headerData(int, Qt::Orientation, int) const;

    void update();

private:
    Peer *rootItem;
};

#endif // PEERMODEL_H
