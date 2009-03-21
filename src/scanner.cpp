#include "scanner.h"

#include "probeftp.h"
#include <QHostAddress>

Scanner::Scanner(QObject *parent, quint32 base, quint32 mask) : QObject(parent), progress (0)
        , number(~mask + 1), base(base & mask) // Clean the base in case it's not.
        , current(base)
{
}

void Scanner::scan()
{
    while (current <= base + qMin(maxProbe, number))
    {
        probe(current);
    }
    emit maximumChanged(number);
}

void Scanner::probeDone()
{
    progress+=1;
    emit progressChanged(progress);
    if (current <= base + number)
    {
        probe(current);
    }
    if (progress >= number)
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
