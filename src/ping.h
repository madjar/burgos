#ifndef PING_H
#define PING_H

#include <QObject>
#include <QString>
#include <QProcess>

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

private:
    QString host;

#ifdef Q_OS_UNIX
private slots:
    void cmdFinished(int exitCode);
    void retry(bool retry);

private:
    QProcess *process;
#endif
};

#endif // PING_H
