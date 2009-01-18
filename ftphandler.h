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
    void print(QTextStream &stream);

public slots:
    void save();
    void addFtp(const QString &host);

signals:
    void itemUpdated(DomItem *);

private:
    QList<Ftp*> list;
    QDomDocument domDocument;
    DomItem *rootItem;
};

#endif // FTPHANDLER_H
