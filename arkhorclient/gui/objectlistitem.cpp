#include "objectlistitem.h"
#include "objectregistry.h"
#include "resourcepool.h"

using namespace AH::Common;

ObjectListItem::ObjectListItem(const QString &objectId)
{
    setData(ObjectIdRole, objectId);
    if (objectId.startsWith("$:")) {
        setText(QString("$%1").arg(objectId.mid(2)));
        setIcon(QPixmap(":/core/marker/dollar"));
    } else {
        ObjectRegistry::instance()->asyncSubscribeObject(this, objectId, AH::Common::RequestObjectsData::Object);
    }
}

void ObjectListItem::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    AH::Common::GameObjectData o;
    desc.data >> o;
    setText(o.name());
    setIcon(ResourcePool::instance()->loadObjectImage(o.id(), o.type()));
}
