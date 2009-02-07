#include "probeftp.h"
#include "ping.h"

#include <QObject>
#include <QString>
#include <QTimer>

#include <QtDebug>

ProbeFtp::ProbeFtp(QString host, QObject *parent) : QObject(parent), host(host)
{
    Ping::ping(host, this, SLOT(pingFinished(int)));
}

ProbeFtp::~ProbeFtp()
{
    //TODO : destroyed() existe déjà
    emit done();
}

void ProbeFtp::pingFinished(int exitCode)
{
    if (exitCode == 0)
    {
        qDebug() << host << "answers to ping";
        ftp = new QFtp(this);
        connect (ftp, SIGNAL(stateChanged(int)),
                 this, SLOT(ftpStateChanged(int)));
        ftp->connectToHost(host);
        ftp->login("anonymous","burgosProbing");
        QTimer::singleShot(timeOut , this, SLOT(deleteLater()));
    }
    else
        deleteLater();
}

//TODO : gestion des buffers dépassés, temps d'attente et tutti cuanti
void ProbeFtp::ftpStateChanged(int state)
{
    if (state == QFtp::LoggedIn)
    {
        emit connected(host);
        ftp->close();
    }
    else if (state == QFtp::Unconnected)
    {
        deleteLater();
    }
}
