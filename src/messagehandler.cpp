#include "messagehandler.h"

#include <QString>

MessageHandler *MessageHandler::singleton_ = NULL;

MessageHandler::MessageHandler()
{
    MessageHandler::oldMsgHandler = qInstallMsgHandler(MessageHandler::staticHandle);
}

MessageHandler::~MessageHandler()
{
    qInstallMsgHandler(MessageHandler::oldMsgHandler);
}

MessageHandler *MessageHandler::pick()
{
    if (MessageHandler::singleton_ == NULL) singleton_ = new MessageHandler();
    return singleton_;
}

void MessageHandler::kill()
{
    delete singleton_;
    singleton_ = NULL;
}

void MessageHandler::staticHandle(QtMsgType type, const char* msg)
{
    switch (type)
    {
    case QtDebugMsg:
        emit MessageHandler::pick()->appendLog(QString("Debug: ") + msg);
        break;
    case QtWarningMsg:
        emit MessageHandler::pick()->appendLog(QString("Warning: ") + msg);
        break;
    case QtCriticalMsg:
        emit MessageHandler::pick()->appendLog(QString("Critical: ") + msg);
        break;
    case QtFatalMsg:
        emit MessageHandler::pick()->appendLog(QString("Fatal: ") + msg);
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

void MessageHandler::appendLogInterface(const QString &string)
{
    emit MessageHandler::appendLog(string);
    fprintf(stdout, "%s", qPrintable(string));
}
