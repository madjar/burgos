#ifndef PING_H
#define PING_H

#include <QObject>
#include <QString>
#include <QProcess>

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping(QString host);
    static Ping *ping(QString host, QObject *receiver, const char *member);

signals:
    void finished(bool answers);

private:
    QString host;
#ifdef Q_OS_UNIX
private slots:
    void cmdFinished(int exitCode);
    void childPing();
    void signalAndSuicide(bool answers);

private:
    QProcess *process;
#endif
};

#endif // PING_H
