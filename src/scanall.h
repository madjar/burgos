#ifndef SCANALL_H
#define SCANALL_H

#include "scanner.h"

#include <QObject>
#include <QMap>
#include <QPair>

class ScanAll : public QObject
{
    Q_OBJECT
public:
    ScanAll();
    void scan();

signals:
    void found(const QString &host);
    void maximumChanged(int value);
    void progressChanged(int value);

private slots:
    void maximum(int value);
    void progress(int value);

private:
    QMap<Scanner *, QPair<int,int> > scanners;
};

#endif // SCANALL_H
