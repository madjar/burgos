#include <QTimer>
#include <QStringList>
#include "cli.h"
#include "scanall.h"

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
    connect(this, SIGNAL(scan()),
            this, SLOT(executeScan()));
}

void Cli::run()
{
    forever
    {
        QString line = in.readLine();
        command(line);
    }
}

void Cli::executeScan()
{
    ScanAll *s = new ScanAll();
    QObject::connect (s, SIGNAL(found(const QString&)),
                    handler, SLOT(addFtp(const QString&)));
    s->scan();
}

void Cli::command(QString cmd)
{
    if (cmd.startsWith("addftp"))
        emit addFtp(cmd.split(' ').at(1));
    else if (cmd=="print")
        emit print(&out);
    else if (cmd=="scan")
        emit scan();
    else
        out << tr("Unknown command : %1").arg(cmd) <<endl;
}
