#include "ftp.h"
#include "domitem.h"

#include <QtCore>
#include <QtNetwork>


#include <QtDebug>

Ftp::Ftp(QString host, DomItem* parent) : QObject(), hostString(host)
{
    root = parent->newChild("ftp");
    root->element().setAttribute("name", host);

    connect (&ftp, SIGNAL(done(bool)),
             this, SLOT(ftpDone(bool)));
    connect (&ftp, SIGNAL(listInfo(const QUrlInfo &)),
             this, SLOT(ftpListInfo(const QUrlInfo &)));
    connect (&ftp, SIGNAL(stateChanged(int)),
             this, SLOT(ftpStateChanged(int)));
    ftp.connectToHost(host, 21);
    qDebug()<<host<<"connected";
}

//Faire le lookup dans le constructeur
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
    root->element().setAttribute("name", host.hostName());
    emit modified(root);
}

void Ftp::updateIndex()
{
    ftp.login("anonymous","burgosIndexing");

    pendingDirs[QString("/")]=root;
    processNextDirectory();
}

void Ftp::processNextDirectory()
{
    if (!pendingDirs.isEmpty()) {
        currentDir = pendingDirs.keys().first();
        currentNode = pendingDirs.value(currentDir);
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
    DomItem *item = currentNode->newChild("file");
    item->element().setAttribute("name", sanitize(urlInfo.name()));
    item->element().setAttribute("size", urlInfo.size());
    if (urlInfo.isDir() && !urlInfo.isSymLink())
    {
        item->element().setTagName("dir");
        pendingDirs[currentDir+'/'+urlInfo.name()] = item;
    }
    emit modified(item);
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

QString Ftp::sanitize(QString string)
{
    // Nettoie les chaînes obtenues par listInfo
    // Problème vient du fait que QFtp récupère les noms de fichier en utf8 et les interprète comme de l'utf16.
    if (string.toLatin1()!=string.toUtf8()) //Basiquement, ça veut dire "si on a un accent"
    {
        const ushort *in = string.utf16();
        char *out = new char[string.length()];
        char *ret = out;
        while (*in)
        {
            if(*in>=256)
                qWarning()<<tr("\"%1\" is not a utf8 string, behavior is not guaranteed.").arg(string);
            *out++=(char)*in++;
        }
        QString sanitized = QString::fromUtf8(ret,string.length());
        delete[] ret;
        return sanitized;
    }
    return string;
}

void Ftp::suicide()
{
    //TODO étudier l'utilité de cette fonction et, le cas échéant, la remettre en état de marche
    //Node::parent()->children.removeAll(this);
    deleteLater();
    qDebug()<<this->hostString<<"ne veut plus vivre";
}
