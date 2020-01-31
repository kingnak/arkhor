#ifndef OBJECTLISTITEM_H
#define OBJECTLISTITEM_H

#include <QListWidgetItem>
#include "asyncobjectreceiver.h"

class ObjectListItem : public QListWidgetItem, public AsyncObjectReceiver
{
public:
    enum {
        ObjectIdRole = Qt::UserRole
    };

    explicit ObjectListItem(QString objectId);

    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);
};

typedef ObjectListItem InventoryListItem;

#endif // OBJECTLISTITEM_H
