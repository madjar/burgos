#include "scanall.h"

#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QList>

ScanAll::ScanAll(QObject *parent) : QObject(parent)
{
}

void ScanAll::scan()
{
    bool empty = true;
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
            if(isValid(entry))
            {
                empty = false;
                scan(entry);
            }
    if (empty)
    {
        qDebug() << tr("No network interface found");
        deleteLater();
    }
}

void ScanAll::scan(quint32 base, quint32 netmask)
{
    Scanner *scanner = new Scanner(this, base, netmask);
    connect(scanner, SIGNAL(found(QString)),
            this, SIGNAL(found(QString)));
    connect(scanner, SIGNAL(maximumChanged(int)),
            this, SLOT(maximum(int)));
    connect(scanner, SIGNAL(progressChanged(int)),
            this, SLOT(progress(int)));
    scanners.insert(scanner, QPair<int,int>());
    scanner->scan();
}

void ScanAll::scan(QNetworkAddressEntry entry)
{
    quint32 netmask = entry.netmask().toIPv4Address();
    quint32 base = entry.ip().toIPv4Address() & netmask;
    scan(base, netmask);
}

void ScanAll::scan(QList<QNetworkAddressEntry> entries)
{
    foreach (QNetworkAddressEntry entry, entries)
        scan(entry);
    if (entries.isEmpty())
        deleteLater();
}

void ScanAll::scanIface(const QString &ifname)
{
    QNetworkInterface iface = QNetworkInterface::interfaceFromName(ifname);
    if (iface.isValid())
    {
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
            if(isValid(entry))
                scan(entry);
    }
    else
        qDebug() << ifname << qPrintable(tr(": invalid network interface"));
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
    bool finished = true;
    QList<QPair<int,int> > pairs = scanners.values();
    for (int i = 0; i<pairs.size(); i++)
    {
        total += pairs.at(i).second;
        finished &= (pairs.at(i).first == pairs.at(i).second);
    }
    emit progressChanged(total);
    if (finished)
        deleteLater();
}
