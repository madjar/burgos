#include "peer.h"

QList<Peer*> Peer::peerList = QList<Peer*>();

Peer::Peer(const QList<QVariant> &data) : itemData(data)
{
    Peer::peerList.append(this);
}

Peer::~Peer()
{
    Peer::peerList.removeAll(this);
}

Peer *Peer::get(int row)
{
    return Peer::peerList.at(row);
}

void Peer::set(const QList<QVariant> &data)
{
    if (data == QList<QVariant>())
    {
        qWarning("__FUNCTION__|__FILE__@__LINE__,: QList<QVariant> vide");
        QList<QVariant> error;
        error << "Erreur";
        this->itemData = error;
        return;
    }
    this->itemData = data;
}

int Peer::row() const
{
    return Peer::peerList.indexOf(const_cast<Peer*>(this));
}

int Peer::rowCount() const
{
    return Peer::peerList.count();
}

int Peer::columnCount() const
{
    return this->itemData.count();
}

QVariant Peer::data(int column) const
{
    return this->itemData.value(column);
}
