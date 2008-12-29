#ifndef FTP_H
#define FTP_H

#include <QHostInfo>
#include <QObject>
#include <QFtp>
#include <QDomNode>
#include <QDomDocument>

class QString;
class QUrlInfo;

class Ftp : public QObject
{
    Q_OBJECT

public:
    Ftp(QString host, QDomDocument doc);

    QVariant data(int column, int role);

public slots:
    void updateIndex();

signals:
    void modified(QDomNode);
    void done();

private slots:
    void ftpDone(bool error);
    void ftpListInfo(const QUrlInfo &urlInfo);
    void setHost(QHostInfo host);
    void ftpStateChanged(int state);

private:
    QHostInfo host;
    QString hostString;
    QDomDocument doc;
    QDomElement root;

    void suicide();
    void processNextDirectory();
    QFtp ftp;
    QString currentDir;
    QDomNode currentNode;
    QMap<QString, QDomNode> pendingDirs;
};

#endif // FTP_H
