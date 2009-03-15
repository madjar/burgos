#include "scanall.h"

#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QList>

//TODO : gestion des limites. Par exemple, si on lance plusieurs scanners, ils ont chacun leur maximum, ce qui peut poser problème.

ScanAll::ScanAll() : QObject(0)
{
}

void ScanAll::scan()
{
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
        {
            if(isValid(entry))
            {
                scan(entry);
            }
        }
    }
}

void ScanAll::scan(QNetworkAddressEntry entry)
{
    quint32 netmask = entry.netmask().toIPv4Address();
    quint32 base = entry.ip().toIPv4Address() & netmask;
    Scanner *scanner = new Scanner(base, netmask);
    connect(scanner, SIGNAL(found(QString)),
            this, SIGNAL(found(QString)));
    connect(scanner, SIGNAL(maximumChanged(int)),
            this, SLOT(maximum(int)));
    connect(scanner, SIGNAL(progressChanged(int)),
            this, SLOT(progress(int)));
    scanners.insert(scanner, QPair<int,int>());
    scanner->scan();
}

bool ScanAll::isValid(QNetworkAddressEntry entry)
{
    return !entry.broadcast().isNull() && entry.ip()!=QHostAddress(QHostAddress::LocalHost);
}

void ScanAll::maximum(int value)
{
    Scanner *s = static_cast<Scanner *>(sender());
    scanners[s].first = value;

    int total = 0;
    QList<QPair<int,int> > pairs = scanners.values();
    for (int i = 0; i<pairs.size(); i++)
    {
        total += pairs.at(i).first;
    }
    emit maximumChanged(total);
}

void ScanAll::progress(int value)
{
    Scanner *s = static_cast<Scanner *>(sender());
    scanners[s].second = value;

    int total = 0;
    QList<QPair<int,int> > pairs = scanners.values();
    for (int i = 0; i<pairs.size(); i++)
    {
        total += pairs.at(i).second;
    }
    emit progressChanged(total);
}
