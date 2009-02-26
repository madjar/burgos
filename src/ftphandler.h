#ifndef FTPHANDLER_H
#define FTPHANDLER_H

#include <QObject>
#include <QDomNode>
#include <QDomDocument>
#include <QString>
#include <QTextStream>

class DomItem;
class Ftp;

class FtpHandler : public QObject
{
    Q_OBJECT
public:
    FtpHandler(QObject *parent = 0);
    ~FtpHandler();

    DomItem *root() const;
    int size() const;

public slots:
    void save();
    void addFtp(const QString &host);
    void print(QTextStream *stream);

signals:
    void itemUpdated(DomItem *);
    void beginAddFtp(int);
    void endAddFtp();

private:
    QList<Ftp*> list;
    QDomDocument domDocument;
    DomItem *rootItem;
};

#endif // FTPHANDLER_H