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

#ifndef PING_H
#define PING_H

#include <QObject>
#include <QString>
#include <QProcess>

#ifdef Q_OS_WIN
const QString pingCmd = QString("ping -n 2 -w 5000 %1");
#else
const QString pingCmd = QString("ping -c 2 %1");
#endif

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping(const QString &host);
    static Ping *ping(const QString &host, QObject *receiver, const char *member);

signals:
    void finished(bool answers);

private slots:
    void signalAndSuicide(bool answers);
    void cmdFinished(int exitCode);
    void retry(bool retry);

private:
    QString host;
    QProcess *process;
};

#endif // PING_H
