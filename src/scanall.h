#ifndef SCANALL_H
#define SCANALL_H

#include "scanner.h"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QNetworkAddressEntry>
#include <QList>

class ScanAll : public QObject
{
    Q_OBJECT
public:
    ScanAll(QObject *parent = 0);
    void scan();
    void scan(quint32 base, quint32 netmask);
    void scan(QNetworkAddressEntry entry);
    void scan(QList<QNetworkAddressEntry> entries);

    static bool isValid(QNetworkAddressEntry entry);

signals:
    void found(const QString &host);
    void maximumChanged(int value);
    void progressChanged(int value);

private slots:
    void maximum(int value);
    void progress(int value);

private:
    QMap<Scanner *, QPair<int,int> > scanners;
};

#endif // SCANALL_H
