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

    explicit ObjectListItem(const QString &objectId);

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;
};

typedef ObjectListItem InventoryListItem;

#endif // OBJECTLISTITEM_H
