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
    connect(this, SIGNAL(scan(const QString &)),
            this, SLOT(executeScan(const QString &)));
}

void Cli::run()
{
    forever
    {
        QString line = in.readLine();
        command(line);
    }
}

void Cli::executeScan(const QString &ifname)
{
    ScanAll *s = new ScanAll();
    QObject::connect (s, SIGNAL(found(const QString&)),
                      handler, SLOT(addFtp(const QString&)));
    if (ifname.isEmpty())
        s->scan();
    else
        s->scanIface(ifname);
}

void Cli::command(QString cmd)
{
    if (cmd.startsWith("addftp"))
        emit addFtp(cmd.split(' ').at(1));
    else if (cmd=="print")
        emit print(&out);
    else if (cmd.startsWith("scan"))
    {
        QStringList splited = cmd.split(' ');
        if (splited.size() >= 2)
            emit scan(splited.at(1));
        else
            emit scan(QString());
    }
    else
        out << tr("Unknown command : %1").arg(cmd) <<endl;
}
