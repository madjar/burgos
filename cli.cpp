#include <QTimer>
#include "cli.h"

#include <QtDebug>

Cli::Cli(QObject *parent) :
        QObject(parent),
        in(stdin, QIODevice::ReadOnly),
        out(stdout, QIODevice::WriteOnly)
{
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
    out<<"You said : "<<cmd<<endl;
}
