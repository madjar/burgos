#ifndef FTPMODEL_H
#define FTPMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QString>


#include "ftphandler.h"

class DomItem;

class FtpModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FtpModel(QObject *parent = 0);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void addFtp(const QString &host);

private slots:
    void itemUpdated(DomItem *item);

private:
    static quint64 recursiveSize(QDomNode node);
    static QString humanReadableSize(qint64 intSize);

    FtpHandler ftpHandler;
};

#endif // BURGOSMODEL_H
