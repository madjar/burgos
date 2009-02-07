#ifndef PING_H
#define PING_H

#include <QObject>
#include <QString>

class Ping : public QObject
{
    Q_OBJECT
public:
    Ping(QString host);
    static Ping *ping(QString host, QObject *receiver, const char *member);

signals:
    void finished(int exitCode);

private:
    QString host;
};

#endif // PING_H
