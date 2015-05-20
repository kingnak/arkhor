#include "registryobjectstackitem.h"
#include "objectregistry.h"
#include "monsterwidget.h"

RegistryObjectStackItem::RegistryObjectStackItem(QString id, QObject *parent) :
    QObject(parent), m_id(id)
{
}

void RegistryObjectStackItem::wasAdded()
{
    if (ObjectRegistry::instance()->hasObject(m_id)) {
        updateObject(ObjectRegistry::instance()->getObject(m_id));
    } else {
        connect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)));
    }
}

void RegistryObjectStackItem::objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription desc)
{
    if (m_id == desc.id) {
        disconnect(ObjectRegistry::instance(), SIGNAL(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)), this, SLOT(objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription)));
        updateObject(desc);
    }
}

//////////////////////////////////////

MonsterStackItem::MonsterStackItem(QString id, QObject *parent)
    : RegistryObjectStackItem(id, parent)
{
    setData(id);
}

void MonsterStackItem::wasAdded()
{
    RegistryObjectStackItem::wasAdded();
    if (m_pixmap.isNull()) {
        AH::Common::MonsterData dummy;
        QPixmap pic = MonsterFrontWidget::drawMonster(&dummy);
        setPixmap(pic);
    }
}

void MonsterStackItem::updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc)
{
    AH::Common::MonsterData m;
    desc.data >> m;
    QPixmap pic = MonsterFrontWidget::drawMonster(&m);
    setPixmap(pic);
}
