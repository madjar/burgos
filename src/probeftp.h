#ifndef PROBEFTP_H
#define PROBEFTP_H

#include <QObject>
#include <QFtp>
#include <QString>

const int timeOut=10000;

class ProbeFtp : public QObject
{
    Q_OBJECT
public:
    ProbeFtp(QString host, QObject *parent = 0);
    ~ProbeFtp();

signals:
    void connected(const QString &host);
    void done();

private slots:
    void pingFinished(int exitCode);
    void ftpStateChanged(int state);

private:
    QString host;
    QFtp *ftp;
};

#endif // PROBEFTP_H
