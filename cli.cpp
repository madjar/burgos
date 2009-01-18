#include <QTimer>
#include <QStringList>
#include "cli.h"

#include <QtDebug>

Cli::Cli(FtpHandler *ftphandler, QObject *parent) :
        QThread(parent),
        in(stdin, QIODevice::ReadOnly),
        out(stdout, QIODevice::WriteOnly),
        handler(ftphandler)
{
    if (!handler)
        handler = new FtpHandler(this);
    connect(this, SIGNAL(addFtp(const QString&)),
            handler, SLOT(addFtp(const QString&)));
    connect(this, SIGNAL(print(QTextStream*)),
            handler, SLOT(print(QTextStream*)));
}

void Cli::run()
{
    forever
    {
        QString line = in.readLine();
        command(line);
    }
}

void Cli::read()
{
    QString line = in.readLine();
    command(line);
}

void Cli::command(QString cmd)
{
    if (cmd.startsWith("addftp"))
        emit addFtp(cmd.split(' ').at(1));
    else if (cmd.contains("print"))
        emit print(&out);
    else
        out << "Command not found" <<endl;
}
