#include "registryobjectstackitem.h"
#include "objectregistry.h"
#include "monsterwidget.h"
#include "resourcepool.h"

RegistryObjectStackItem::RegistryObjectStackItem(QString id) :
    m_id(id)
{
}

void RegistryObjectStackItem::wasAdded()
{
    ObjectRegistry::instance()->asyncGetObject(this, m_id);
}

void RegistryObjectStackItem::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    updateObject(desc);
}

//////////////////////////////////////

MonsterStackItem::MonsterStackItem(QString id)
    : RegistryObjectStackItem(id)
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

/////////////////////////////////////

CharacterStackItem::CharacterStackItem(QString id)
{
    setData(id);
    setPixmap(ResourcePool::instance()->loadCharacterFigure(id));
}

