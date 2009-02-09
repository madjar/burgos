#include "utils.h"

#include <QCoreApplication>
#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QProcess>

#include <QtDebug>

bool Utils::bufferProblem()
{
    QString cmd = QCoreApplication::arguments().at(0) + " --increase-buffer";

    QMessageBox msg;
    msg.setText(QCoreApplication::translate("Utils","Connection limit reached"));
    msg.setInformativeText(QCoreApplication::translate("Utils","Burgos isn't able to scan any more servers. "
                              "In order to solve this problem, Burgos needs to modify one of your kernel's settings."));
    msg.setDetailedText(QCoreApplication::translate("Utils","For the nerdy ones, the problem is that ping says there no buffer space available. We're going to increase the size of the arp table by running that command :\n"
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
            QMessageBox::critical(0, QCoreApplication::translate("Utils","Unable to execute the command"), QCoreApplication::translate("Utils","Please run this command yourself :\n%1").arg(cmd));
            return false;
        default:
            return QProcess::execute(list.at(pos) + ' ' + '"' + cmd + '"');
        }
    }
    return false;
}
