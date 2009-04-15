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
