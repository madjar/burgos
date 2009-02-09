#include "scanftp.h"
#include "probeftp.h"

#include <QObject>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>
#include <QString>
#include <QTimer>

#include <QtDebug>

ScanFtp::ScanFtp(QObject *parent) : QObject(parent)
{
    opened = 0;
    progress = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(next()));
    timer->start(10);
}

void ScanFtp::scan()
{
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
            if(!entry.broadcast().isNull() && entry.ip()!=QHostAddress(QHostAddress::LocalHost))
            {
                quint32 ip= entry.ip().toIPv4Address();
                quint32 netmask = entry.netmask().toIPv4Address();
                quint32 base = ip & netmask;
                for (quint32 current = 0x00000000; current <= (~netmask); current++)
                    if (current % 256 != 0 && current % 256 != 255)
                        list.append(QHostAddress(base + current).toString());
            }
    emit maximumChanged(list.size());
}

void ScanFtp::next()
{
    if (opened >= maxSockets)
        return;
    if (!list.isEmpty())
    {
        opened +=1;
        knock(list.takeFirst());
    }
    else if (opened == 0)
        deleteLater();
}

void ScanFtp::knock(const QString &host)
{
    ProbeFtp *probe = new ProbeFtp(host, this);
    connect(probe, SIGNAL(connected(const QString&)),
            this, SIGNAL(found(const QString&)));
    connect(probe, SIGNAL(done()),
            this, SLOT(probeDone()));
}

void ScanFtp::probeDone()
{
    opened-=1;
    progress+=1;
    emit progressChanged(progress);
}

