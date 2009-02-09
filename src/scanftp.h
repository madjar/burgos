#ifndef SCANFTP_H
#define SCANFTP_H

#include <QObject>
#include <QHostInfo>
#include <QStringList>

const int maxSockets = 128;

class ScanFtp : public QObject
{
    Q_OBJECT

public:
    ScanFtp(QObject *parent = 0);
    void scan();

signals:
    void found(const QString &host);
    void maximumChanged(int value);
    void progressChanged(int value);

private slots:
    void next();
    void probeDone();

private:
    void knock(const QString &host);

    QStringList list;
    int opened;
    int progress;
};

#endif // SCANFTP_H
