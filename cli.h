#ifndef CLI_H
#define CLI_H

#include <QObject>
#include <QTextStream>
#include <QString>

#include "ftphandler.h"

class Cli : public QObject
{
    Q_OBJECT

public:
    Cli(FtpHandler *handler = 0, QObject *parent = 0);

public slots:
    void read();

private:
    void command(QString cmd);

    QTextStream in;
    QTextStream out;
    FtpHandler *handler;
};

#endif // CLI_H
