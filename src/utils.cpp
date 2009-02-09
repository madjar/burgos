#include "utils.h"

#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>

int Utils::increaseBuffer()
{
    int result =  QProcess::execute("/sbin/sysctl -w "
                                    "net.ipv4.neigh.default.gc_thresh3=4096 "
                                    "net.ipv4.neigh.default.gc_thresh2=2048 "
                                    "net.ipv4.neigh.default.gc_thresh1=1024");
    QTextStream out(stdout, QIODevice::WriteOnly);
    if (!result)
        out<< QCoreApplication::translate("Utils","Buffer size increased.\n");
    else
        out<< QCoreApplication::translate("Utils","Operation failed, please check you've got admin rights.\n");
    return result;
}
