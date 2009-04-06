#ifndef DEF_H
#define DEF_H

namespace Burgos
{
    enum BurgosRole
    {
        // Contains sortable values, like real sizes instead of human-readable
        SortRole = Qt::UserRole,
        // The url of a file that respresented as an item
        UrlRole = Qt::UserRole + 1
    };
}
#endif // DEF_H
