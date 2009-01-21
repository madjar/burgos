#include "probeftp.h"

#include <QObject>
#include <QString>
#include <QTimer>

#include <QtDebug>

ProbeFtp::ProbeFtp(QString host, QObject *parent) : QObject(parent), host(host)
{
    ftp = new QFtp();
    connect (ftp, SIGNAL(stateChanged(int)),
             this, SLOT(ftpStateChanged(int)));
    ftp->connectToHost(host);
    ftp->login("anonymous","burgosProbing");
    QTimer::singleShot(timeOut , this, SLOT(deleteLater()));

}

ProbeFtp::~ProbeFtp()
{
    if (ftp)
    {
        //qWarning()<<host<< tr("Socket is still existing but should have been destroyed before");
        ftp->close();
        ftp->deleteLater();
        ftp = 0;
    }
    emit done();
}

void ProbeFtp::ftpStateChanged(int state)
{
    if (state == QFtp::LoggedIn)
    {
        emit connected(host);
        ftp->close();
    }
    else if (state == QFtp::Unconnected)
    {
        //qDebug()<<host<<ftp->error();
        deleteLater();
    }
}
