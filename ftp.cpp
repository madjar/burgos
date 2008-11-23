#include "ftp.h"

#include <QtCore>
#include <QPixmap>
#include <QtNetwork>

#include "node.h"
#include "file.h"

Ftp::Ftp(QHostInfo host) : QObject(),
File(QUrlInfo()),  host(host)
{
    init();
}

Ftp::Ftp(QString host) : QObject(),
File(QUrlInfo()), host(QHostInfo::fromName(host))
{
    init();
}

void Ftp::init()
{
    connect (&ftp, SIGNAL(done(bool)),
             this, SLOT(ftpDone(bool)));
    connect (&ftp, SIGNAL(listInfo(const QUrlInfo &)),
             this, SLOT(ftpListInfo(const QUrlInfo &)));
    updateIndex();
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
    ftp.connectToHost(host.hostName(), 21);
    ftp.login();

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
    File *f = new File(urlInfo);
    currentFile->addChild(f);
    if (urlInfo.isDir() && !urlInfo.isSymLink())
        pendingDirs[currentDir+'/'+urlInfo.name()] = f;
}

void Ftp::ftpDone(bool /*error*/)
{
    processNextDirectory();
}
