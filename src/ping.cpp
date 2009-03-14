#include "ping.h"
#include "buffererrorhandler.h"

#include <QProcess>
#include <QtDebug>

Ping::Ping(const QString &host) : QObject(0), host(host)
{
    process = new QProcess(this);
    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(cmdFinished(int)));
    process->start(pingCmd.arg(host), QIODevice::ReadOnly);
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
#ifdef Q_OS_WIN
        qDebug() << qPrintable(host) << " : "<< error.constData();
        signalAndSuicide(false);
#else
        if(error.contains("connect: No buffer space available"))
        {
            BufferErrorHandler::handle(this, SLOT(retry(bool)));
        }
        else
        {
            qDebug()<<qPrintable(host)<<" : "<< error.constData();
            signalAndSuicide(false);
        }
#endif
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
