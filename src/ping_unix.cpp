#include "ping.h"
#include "buffererrorhandler.h"

#include <QProcess>

Ping::Ping(const QString &host) : QObject(0), host(host)
{
    process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(cmdFinished(int)));
    process->start(QString("ping -c 3 %1").arg(host), QIODevice::ReadOnly);
}

Ping *Ping::ping(const QString &host, QObject *receiver, const char *member)
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
            BufferErrorHandler::handle(this, SLOT(retry(bool)));
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

void Ping::retry(bool retry)
{
    disconnect(sender(), 0, this, 0);
    if (retry)
        Ping::ping(host, this, SLOT(signalAndSuicide(bool)));
    else
        signalAndSuicide(false);
}

void Ping::signalAndSuicide(bool answers)
{
    emit finished(answers);
    deleteLater();
}
