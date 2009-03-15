#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>

#ifdef Q_OS_WIN
const int maxProbe = 60;
#else
const int maxProbe = 128;
#endif

class Scanner : public QObject
{
    Q_OBJECT

public:
    Scanner(quint32 base, quint32 mask);
    Scanner(const QString &subnet);
    void scan();

signals:
    void found(const QString &host);
    void maximumChanged(int value);
    void progressChanged(int value);

private slots:
    void probeDone();

private:
    void probe(quint32 host);

    quint32 progress;
    const quint32 mask;
    const quint32 base;
    quint32 current;
};

#endif // SCANNER_H
