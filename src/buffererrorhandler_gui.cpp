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

#include "buffererrorhandler.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QProcess>

#include <QtDebug>

bool BufferErrorHandler::bufferProblem()
{
    QString cmd = QCoreApplication::arguments().at(0) + " --increase-buffer";

    QMessageBox msg;
    msg.setText(tr("Connection limit reached"));
    msg.setInformativeText(tr("Burgos isn't able to scan any more servers. "
                              "In order to solve this problem, Burgos needs to modify one of your kernel's settings."));
    msg.setDetailedText(tr("For the nerdy ones, the problem is that ping says there no buffer space available. We're going to increase the size of the arp table by running that command :\n"
                           "%1").arg(cmd));
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Abort);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Warning);
    int ret = msg.exec();
    if (ret == QMessageBox::Ok)
    {
        //TODO exploiter les spécificitées des différentes commandes pour avoir un résultat pas trop moche.
        QStringList list;
        list << "kdesudo -c" << "gksudo" << "kdesu -c" << "gksu";
        int pos;
        for (pos=0; pos<list.size(); pos++)
        {
            if(!QProcess::execute("which", QStringList(list.at(pos).split(' ').at(0))))
                break;
        }

        switch(pos)
        {
        case 4: // Rien
            QMessageBox::critical(0, tr("Unable to execute the command"), tr("Please run this command yourself :\n%1").arg(cmd));
            return false;
        default:
            return QProcess::execute(list.at(pos) + ' ' + '"' + cmd + '"');
        }
    }
    return false;
}
