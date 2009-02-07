#include "ping.h"

#include <QProcess>

Ping::Ping(QString host) : QObject(0), host(host)
{
    QProcess *process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SIGNAL(finished(int)));
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(deleteLater()));
    process->start(QString("ping -c 3 %1").arg(host), QIODevice::NotOpen);
}

//TODO : gestion des crashs de ping pour problèmes de buffer
// note : exit code de ping : 0=up, 1=down, 2=error

Ping *Ping::ping(QString host, QObject *receiver, const char *member)
{
    Ping *ping = new Ping(host);
    connect(ping, SIGNAL(finished(int)), receiver, member);
    return ping;
}
