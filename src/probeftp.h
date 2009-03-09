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
    ProbeFtp(const QString &host, QObject *parent = 0);

signals:
    void connected(const QString &host);

private slots:
    void pingFinished(bool answers);
    void ftpStateChanged(int state);

private:
    QString host;
    QFtp *ftp;
};

#endif // PROBEFTP_H
