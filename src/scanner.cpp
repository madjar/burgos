#include "scanner.h"

#include "probeftp.h"
#include <QHostAddress>
#include <cmath>


Scanner::Scanner(quint32 base, quint32 mask) : QObject(0), progress (0)
        , mask(mask), base(base & mask) // Clean the base in case it's not.
        , current(base)
{
}

Scanner::Scanner(const QString &subnet) : QObject(0), progress (0)
        , mask(~((quint32) pow(2,32 - QHostAddress::parseSubnet(subnet).second)-1))
        , base(QHostAddress::parseSubnet(subnet).first.toIPv4Address())
        , current(base)
{
}

void Scanner::scan()
{
    while (current < base + maxProbe)
    {
        probe(current);
    }
    emit maximumChanged(~mask);
}

void Scanner::probeDone()
{
    progress+=1;
    emit progressChanged(progress);
    if (current <= base + ~mask)
    {
        probe(current);
    }
    if (progress == ~mask)
    {
        deleteLater();
    }
}

void Scanner::probe(quint32 host)
{
    if (current % 256 == 0 || current % 256 == 255)
    {
        // These aren't valid adresses, we just skip them.
        current +=1;
        progress +=1;
    }
    else
    {
        current += 1;
        ProbeFtp *probe = new ProbeFtp(QHostAddress(host).toString(), this);
        connect(probe, SIGNAL(connected(const QString&)),
                this, SIGNAL(found(const QString&)));
        connect(probe, SIGNAL(destroyed()),
                this, SLOT(probeDone()));
    }
}


