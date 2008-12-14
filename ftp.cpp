#include "ftp.h"

#include <QtCore>
#include <QPixmap>
#include <QtNetwork>

#include "node.h"
#include "file.h"

#include <QtDebug>

Ftp::Ftp(QString host) : QObject(), File(QUrlInfo())
{
    hostString=host;
    connect (&ftp, SIGNAL(done(bool)),
             this, SLOT(ftpDone(bool)));
    connect (&ftp, SIGNAL(listInfo(const QUrlInfo &)),
             this, SLOT(ftpListInfo(const QUrlInfo &)));
    connect (&ftp, SIGNAL(stateChanged(int)),
             this, SLOT(ftpStateChanged(int)));
    ftp.connectToHost(host, 21);
    qDebug()<<host<<tr("connected");
}

void Ftp::ftpStateChanged(int state)
{
    if (state == QFtp::Connected)
    {
        QHostInfo::lookupHost(hostString,
                              this, SLOT(setHost(QHostInfo)));
        updateIndex();
    }
}

void Ftp::setHost(QHostInfo host)
{
    if (host.error() != QHostInfo::NoError)
    {
        qDebug()<< host.hostName()<< "n'a pas pu être ajouté parce que :"<<host.error();
        suicide();
        return;
    }
    this->host=host;
}

QVariant Ftp::data(int column, int role)
{
    if (role == Qt::TextAlignmentRole && column == 1)
        return Qt::AlignRight;

    if (role == Qt::DecorationRole && column == 0)
        return QPixmap(":/icons/computer.png");

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole)
        return QVariant();

    switch (column)
    {
    case 0:
        return host.hostName();
    case 1:
        return File::humanReadableSize(size());
    }
    return QVariant();
}

void Ftp::updateIndex()
{
    ftp.login("anonymous","burgosIndexing");

    pendingDirs[QString("/")]=this;
    processNextDirectory();
}

void Ftp::processNextDirectory()
{
    if (!pendingDirs.isEmpty()) {
        currentDir = pendingDirs.keys().first();
        currentFile = pendingDirs.value(currentDir);
        pendingDirs.remove(currentDir);

        ftp.cd(currentDir);
        ftp.list();
    } else {
        emit done();
        ftp.close();
    }
}

void Ftp::ftpListInfo(const QUrlInfo &urlInfo)
{
    if (urlInfo.name()=="." || urlInfo.name()=="..")
        return;
    File *f = new File(urlInfo);
    currentFile->addChild(f);
    if (urlInfo.isDir() && !urlInfo.isSymLink())
    //if (urlInfo.isDir())
        pendingDirs[currentDir+'/'+urlInfo.name()] = f;
    emit modified(f);
}

void Ftp::ftpDone(bool error)
{
    if (error) // Pas forcément propre, mais c'est une manière de dégommer les ftp non ouverts ou privés
    {
        //suicide();
        qDebug()<<ftp.errorString();
    }
    if (ftp.state()!=QFtp::Unconnected)
        processNextDirectory();
}

void Ftp::suicide()
{
    Node::parent->children.removeAll(this);
    deleteLater();
    qDebug()<<this->hostString<<"ne veut plus vivre";
}
