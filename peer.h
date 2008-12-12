#ifndef PEER_H
#define PEER_H

#include <QVariant>
#include <QList>

/*
 * Peer.h
 * QList de pairs
 * la liste est fondue dans la classe meme sous forme de membre statique
 */
class Peer
{
public:
    Peer(const QList<QVariant> &);
    ~Peer();

public:
    Peer *get(int);
    void set(const QList<QVariant> &);
    int row() const;
    int rowCount() const;
    int columnCount() const;
    QVariant data(int) const;

private:
    static QList<Peer*> peerList;
    QList<QVariant> itemData;
};

#endif // PEER_H
