#ifndef FILE_H
#define FILE_H

#include "node.h"
#include <QUrlInfo>


class File : public Node
{
public:
    File(QUrlInfo urlInfo);
    virtual ~File();

    virtual QVariant data(int column, int role);

    virtual bool isDir();
    qint64 size();

    static QString humanReadableSize(qint64 size);

private:
    QUrlInfo urlInfo;
};

#endif // FILE_H
