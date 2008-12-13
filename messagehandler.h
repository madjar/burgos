#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QObject>
#include <QString>
#include <QtDebug>

class MessageHandler : public QObject
{
    Q_OBJECT

private:
    static MessageHandler *singleton_;

private:
     MessageHandler();
    ~MessageHandler();

public:
    static MessageHandler *pick();
    static void            kill();

public slots:
    void appendLogInterface(const QString &);

signals:
    void appendLog(const QString &);

public:
    static void staticHandle(QtMsgType, const char *);

private:
    QtMsgHandler oldMsgHandler;
};

#endif // MESSAGEHANDLER_H
