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

#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>

#ifdef Q_OS_LINUX
const quint32 maxProbe = 128;
#elif Q_OS_MAC
const quint32 maxProbe = 50;
#else
const quint32 maxProbe = 60;
#endif

class Scanner : public QObject
{
    Q_OBJECT

public:
    Scanner(QObject *parent, quint32 base, quint32 mask);
    void scan();

signals:
    void found(const QString &host);
    void maximumChanged(int value);
    void progressChanged(int value);

private slots:
    void probeDone();

private:
    void probe(quint32 host);

    quint32 progress;
    const quint32 number;
    const quint32 base;
    quint32 current;
};

#endif // SCANNER_H
