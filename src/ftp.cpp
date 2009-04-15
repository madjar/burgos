/*

Copyright 2008 by Thomas Duboucher and Georges Dubus.

thomas.duboucher@supelec.fr
georges.dubus@supelec.fr

This software is a computer program whose purpose is to scan and
index ftp servers.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.

*/

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

QString Ftp::sanitize(const QString &string)
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
            //if (*in == 232 || (*in >= 232 && *in <= 235))
            if ((*in & 0xC0) == 0xC0 && (*(in+1) & 0xC0) != 0x80)
            {
                // This ain't not utf8, ma'am, this is fracking latin9
                // See http://en.wikipedia.org/wiki/Utf8#Description for details
                return string;
            }
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
