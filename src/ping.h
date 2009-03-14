#ifndef PING_H
#define PING_H

#include <QObject>
#include <QString>
#include <QProcess>

#ifdef Q_OS_WIN
const QString pingCmd = QString("ping -n 2 %1");
#else
const QString pingCmd = QString("ping -c 2 %1");
#endif

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping(const QString &host);
    static Ping *ping(const QString &host, QObject *receiver, const char *member);

signals:
    void finished(bool answers);

private slots:
    void signalAndSuicide(bool answers);
    void cmdFinished(int exitCode);
    void retry(bool retry);

private:
    QString host;
    QProcess *process;
};

#endif // PING_H
