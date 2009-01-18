#include <QTimer>
#include <QStringList>
#include "cli.h"

#include <QtDebug>

Cli::Cli(FtpHandler *handler, QObject *parent) :
        QObject(parent),
        in(stdin, QIODevice::ReadOnly),
        out(stdout, QIODevice::WriteOnly),
        handler(handler)
{
    if (!handler)
        this->handler = new FtpHandler(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(read()));
    timer->start();
}

void Cli::read()
{
    QString line = in.readLine();
    command(line);
}

void Cli::command(QString cmd)
{
    if (cmd.startsWith("addftp"))
        handler->addFtp(cmd.split(' ').at(1));
    else if (cmd.contains("print"))
        handler->print(out);
}
