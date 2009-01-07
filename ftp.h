#ifndef FTP_H
#define FTP_H

#include <QHostInfo>
#include <QObject>
#include <QFtp>
#include <QDomNode>
#include <QDomDocument>

class QString;
class QUrlInfo;
class DomItem;

class Ftp : public QObject
{
    Q_OBJECT

public:
    Ftp(QString host, DomItem *doc);
    
public slots:
    void updateIndex();

signals:
    void modified(DomItem*);
    void done();

private slots:
    void ftpDone(bool error);
    void ftpListInfo(const QUrlInfo &urlInfo);
    void setHost(QHostInfo host);
    void ftpStateChanged(int state);

private:
    QHostInfo host;
    QString hostString;
    DomItem *root;

    static QString sanitize(QString string);

    void suicide();
    void processNextDirectory();
    QFtp ftp;
    QString currentDir;
    DomItem *currentNode;
    QMap<QString, DomItem*> pendingDirs;
};

#endif // FTP_H
