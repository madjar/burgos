#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QString>

class Node;

class Model : public QAbstractItemModel
{
public:
    Model(QObject *parent = 0);
    ~Model();

    void setRootNode(Node *node);
    void addFtp(QString host);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    Node *nodeFromIndex(const QModelIndex &index) const;
    Node *rootNode;
};

#endif // BURGOSMODEL_H
