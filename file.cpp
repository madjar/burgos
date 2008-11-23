#include "file.h"
#include "node.h"

#include <QUrlInfo>
#include <QPixmap>

File::File(QUrlInfo urlInfo) : Node(), urlInfo(urlInfo)
{
}

QVariant File::data(int column, int role)
{
    if (role == Qt::TextAlignmentRole && column == 1)
        return Qt::AlignRight;

    if (role == Qt::DecorationRole && column == 0)
        if (urlInfo.isDir())
            return QPixmap(":/icons/directory.png");

    if (role != Qt::DisplayRole && role != Qt::ToolTipRole)
        return QVariant();
    switch (column)
    {
    case 0:
        return urlInfo.name();
    case 1:
        return humanReadableSize(size());
    }

    return QVariant();
}

bool File::isDir()
{
    return urlInfo.isDir();
}

qint64 File::size()
{
    if (children.isEmpty())
        return urlInfo.size();
    else
    {
        qint64 total=0;
        foreach (Node *child, children)
        {
            File *file = static_cast<File *>(child);
            if (file)
                total += file->size();
        }
        return total;
    }
}

QString File::humanReadableSize(qint64 intSize)
{
    static const QStringList prefix= QStringList() << "" << "K" << "M" << "G" << "T" << "P" << "E" << "Z" << "Y";
    // TODO : potential bug if size > 1024^8
    double size=intSize;
    int exponent=0;
    while (size>1024)
    {
        size /= 1024;
        exponent +=1;
    }

    return QString::number(size, 'f', 1) + " " + prefix.at(exponent) + "o";
}
