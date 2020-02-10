#include "registryobjectstackitem.h"
#include "objectregistry.h"
#include "monsterwidget.h"
#include "characterwidget.h"
#include "resourcepool.h"

RegistryObjectStackItem::RegistryObjectStackItem(QString id, bool subscribe) :
    m_id(id)
  , m_subscribing(subscribe)
{
}

void RegistryObjectStackItem::wasAdded()
{
    if (m_subscribing)
        ObjectRegistry::instance()->asyncSubscribeObject(this, m_id);
    else
        ObjectRegistry::instance()->asyncGetObject(this, m_id);
}


void RegistryObjectStackItem::wasRemoved()
{
    ObjectRegistry::instance()->unsubscribe(this);
}

void RegistryObjectStackItem::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    updateObject(desc);
}

//////////////////////////////////////

MonsterStackItem::MonsterStackItem(QString id, double scale)
    : RegistryObjectStackItem(id)
    , m_scale(scale)
{
    setId(id);
    setData(id);
}

void MonsterStackItem::wasAdded()
{
    RegistryObjectStackItem::wasAdded();
    if (m_pixmap.isNull()) {
        AH::Common::MonsterData dummy;
        QPixmap pic = MonsterFrontWidget::drawMonster(&dummy, m_scale);
        setPixmap(pic);
    }
}

void MonsterStackItem::updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc)
{
    AH::Common::MonsterData m;
    desc.data >> m;
    QPixmap pic = MonsterFrontWidget::drawMonster(&m, m_scale);
    setPixmap(pic);
}

/////////////////////////////////////

CharacterStackItem::CharacterStackItem(QString id, double scale)
    : RegistryObjectStackItem(id, true)
    , m_scale(scale)
{
    setId(id);
    setData(id);
    setPixmap(ResourcePool::instance()->loadCharacterFigure(id));
}

void CharacterStackItem::wasAdded()
{
    RegistryObjectStackItem::wasAdded();
    if (m_pixmap.isNull()) {
        AH::Common::CharacterData dummy;
        QPixmap pic = ResourcePool::instance()->loadCharacterFigure(this->m_id);
        setPixmap(pic);
    }
}

void CharacterStackItem::updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc)
{
    AH::Common::CharacterData c;
    desc.data >> c;
    QPixmap pic = CharacterWidget::drawCharacterWithStats(&c, m_scale);
    setPixmap(pic);
}
