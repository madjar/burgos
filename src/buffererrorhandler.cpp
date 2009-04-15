#include "buffererrorhandler.h"

#include <QCoreApplication>
#include <QProcess>
#include <QTextStream>

BufferErrorHandler *BufferErrorHandler::singleton = 0;

BufferErrorHandler::BufferErrorHandler() : QObject(0), state(-2)
{
}

void BufferErrorHandler::handle(QObject *receiver, const char *member)
{
    if (!singleton)
        singleton = new BufferErrorHandler();
    connect(singleton, SIGNAL(handled(bool)),
            receiver, member);
    singleton->printRequest();
}

void BufferErrorHandler::printRequest()
{
    if (state == -2)
    {
        state = -1;
        state = bufferProblem(); // Vu qu'on peut utiliser des QMessageBox, la réponse n'est pas immédiate
        emit handled(state);
    }
    else if (state >= 0)
        emit handled(state);
}

int BufferErrorHandler::increaseBuffer()
{
    int result =  QProcess::execute("/sbin/sysctl -w "
                                    "net.ipv4.neigh.default.gc_thresh3=4096 "
                                    "net.ipv4.neigh.default.gc_thresh2=2048 "
                                    "net.ipv4.neigh.default.gc_thresh1=1024");
    QTextStream out(stdout, QIODevice::WriteOnly);
    if (!result)
        out<< tr("Buffer size increased.\n");
    else
        out<< tr("Operation failed, please check you've got admin rights.\n");
    return result;
}
