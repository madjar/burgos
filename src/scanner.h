#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>

#ifdef Q_OS_WIN
const quint32 maxProbe = 60;
#else
const quint32 maxProbe = 128;
#endif

class Scanner : public QObject
{
    Q_OBJECT

public:
    Scanner(QObject *parent, quint32 base, quint32 mask);
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
    const quint32 number;
    const quint32 base;
    quint32 current;
};

#endif // SCANNER_H
