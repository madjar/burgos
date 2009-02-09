#include "ping.h"
#include "utils.h"

#include <QtDebug>
#include <QProcess>

Ping::Ping(QString host) : QObject(0), host(host)
{
    process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(cmdFinished(int)));
    process->start(QString("ping -c 3 %1").arg(host), QIODevice::ReadOnly);
}

Ping *Ping::ping(QString host, QObject *receiver, const char *member)
{
    Ping *ping = new Ping(host);
    connect(ping, SIGNAL(finished(bool)), receiver, member);
    return ping;
}

void Ping::cmdFinished(int exitCode)
{
    if (exitCode==2)
    {
        QByteArray error = process->readAllStandardError();
        if(error.contains("connect: No buffer space available"))
        {
            qDebug()<<host<<"No buffer space available";
            if (Utils::bufferProblem())
                childPing();
            else
                signalAndSuicide(false);
        }
        else
        {
            qDebug()<<host<<"Unknown error :"<< error;
            signalAndSuicide(false);
        }
    }
    else
    {
        signalAndSuicide(!exitCode); // if exitCode==0, host is up and answers is true
    }
}

void Ping::childPing()
{
    Ping::ping(host, this, SLOT(signalAndSuicide(bool)));
}

void Ping::signalAndSuicide(bool answers)
{
    emit finished(answers);
    deleteLater();
}
