#include "messagehandler.h"

#include <QString>

messageHandler *messageHandler::singleton_ = NULL;

messageHandler::messageHandler()
{
    messageHandler::oldMsgHandler = qInstallMsgHandler(messageHandler::staticHandle);
}

messageHandler::~messageHandler()
{
    qInstallMsgHandler(messageHandler::oldMsgHandler);
}

messageHandler *messageHandler::pick()
{
    if (messageHandler::singleton_ == NULL) singleton_ = new messageHandler();
    return singleton_;
}

void messageHandler::kill()
{
    delete singleton_;
    singleton_ = NULL;
}

void messageHandler::staticHandle(QtMsgType type, const char* msg)
{
    switch (type)
    {
    case QtDebugMsg:
        emit messageHandler::pick()->appendLog(QString("Debug: ") + msg);
        break;
    case QtWarningMsg:
        emit messageHandler::pick()->appendLog(QString("Warning: ") + msg);
        break;
    case QtCriticalMsg:
        emit messageHandler::pick()->appendLog(QString("Critical: ") + msg);
        break;
    case QtFatalMsg:
        emit messageHandler::pick()->appendLog(QString("Fatal: ") + msg);
        abort();
    }
    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}

void messageHandler::appendLogInterface(const QString &string)
{
    emit messageHandler::appendLog(string);
    fprintf(stdout, qPrintable(string));
}
