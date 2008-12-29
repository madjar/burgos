#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QString>
#include <QDomNode>
#include <QDomDocument>

#include "ftp.h"

class Node;

class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    Model(QObject *parent = 0);
    ~Model();

    //void setRootNode(QDomNode node);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void addFtp(QString &host);

private slots:
    void nodeUpdated(QDomNode node);

private:
    QDomNode nodeFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromNode(QDomNode node, int column);

    QList<Ftp*> list;
    QDomDocument document;
};

#endif // BURGOSMODEL_H
