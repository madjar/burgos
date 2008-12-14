#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QString>

class Node;

class Model : public QAbstractItemModel
{
    Q_OBJECT
public:
    Model(QObject *parent = 0);
    ~Model();

    void setRootNode(Node *node);

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
    void nodeUpdated(Node *node);

private:
    Node *nodeFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromNode(Node *node, int column);
    Node *rootNode;
};

#endif // BURGOSMODEL_H
