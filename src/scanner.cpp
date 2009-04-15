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
