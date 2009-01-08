#ifndef FTPMODEL_H
#define FTPMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QString>
#include <QDomNode>
#include <QDomDocument>

#include "ftp.h"

class DomItem;

class FtpModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FtpModel(QObject *parent = 0);
    ~FtpModel();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void save();
    void addFtp(QString &host);

private slots:
    void itemUpdated(DomItem *item);

private:
    static quint64 recursiveSize(QDomNode node);
    static QString humanReadableSize(qint64 intSize);

    QList<Ftp*> list;
    QDomDocument domDocument;
    DomItem *rootItem;
};

#endif // BURGOSMODEL_H
