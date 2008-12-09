#ifndef FTP_H
#define FTP_H

#include "file.h"
#include <QHostInfo>
#include <QObject>
#include <QFtp>
class QString;
class QUrlInfo;

class Ftp : public QObject, public File
{
    Q_OBJECT

public:
    Ftp(QString host);

    QVariant data(int column, int role);

public slots:
    void updateIndex();

signals:
    void modified();
    void done();

private slots:
    void ftpDone(bool error);
    void ftpListInfo(const QUrlInfo &urlInfo);
    void setHost(QHostInfo host);
    void ftpStateChanged(int state);

private:
    QHostInfo host;
    QString hostString;

    void suicide();
    void processNextDirectory();
    QFtp ftp;
    QString currentDir;
    File *currentFile;
    QMap<QString, File*> pendingDirs;
};

#endif // FTP_H
