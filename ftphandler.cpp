#include <QtCore>
#include "ftphandler.h"
#include "ftp.h"
#include "domitem.h"

FtpHandler::FtpHandler(QObject *parent) : QObject(parent), domDocument(QDomDocument("ftp_index"))
{
        QDomElement domRoot = domDocument.createElement("ftp_list");
    rootItem = new DomItem(domRoot, 0);
    domDocument.appendChild(domRoot);
    connect(qApp, SIGNAL(aboutToQuit()),
            this, SLOT(save()));
}

FtpHandler::~FtpHandler()
{
    delete rootItem;
}

DomItem *FtpHandler::root() const
{
    return rootItem;
}

int FtpHandler::size() const
{
    return list.size();
}

void FtpHandler::addFtp(QString &host)
{
    Ftp *ftp = new Ftp(host, rootItem);
    list.append(ftp);
    connect(ftp,SIGNAL(modified(DomItem*)),
            this, SIGNAL(itemUpdated(DomItem*)));
}

void FtpHandler::save()
{
    const QString dir = QDir::homePath()+"/.burgos/";
    QDir d;
    if (!d.exists(dir))
        d.mkdir(dir);
    QFile file(dir + "index.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    domDocument.save(out, 4);
    file.close();
}
