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
