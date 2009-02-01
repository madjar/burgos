#ifndef CLI_H
#define CLI_H

#include <QObject>
#include <QThread>
#include <QTextStream>
#include <QString>

#include "ftphandler.h"

class Cli : public QThread
{
    Q_OBJECT

public:
    Cli(FtpHandler *ftphandler = 0, QObject *parent = 0);
    void run();

public slots:
    void executeScan();

signals:
    void addFtp (const QString&);
    void print (QTextStream*);
    void scan();

private:
    void command(QString cmd);

    QTextStream in;
    QTextStream out;
    FtpHandler *handler;
};

#endif // CLI_H
