#include "buffererrorhandler.h"

#include <QCoreApplication>
#include <QStringList>
#include <QTextStream>

bool BufferErrorHandler::bufferProblem()
{
    QTextStream (stdout, QIODevice::WriteOnly)
            <<QCoreApplication::translate("Utils","Burgos isn't able to scan any more servers. "
                                          "In order to solve this problem, Burgos needs you to modify one of your kernel's settings.")
            << endl
            << QCoreApplication::translate("Utils","Please run this command as root :\n"
                                           "%1").arg(QCoreApplication::arguments().at(0) + " --increase-buffer");
    return false;
}
