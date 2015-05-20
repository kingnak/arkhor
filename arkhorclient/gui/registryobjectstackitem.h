#ifndef REGISTRYOBJECTSTACKITEM_H
#define REGISTRYOBJECTSTACKITEM_H

#include "itemstacker.h"
#include <objectdata.h>
#include "asyncobjectreceiver.h"

class RegistryObjectStackItem : public StackItem, public AsyncObjectReceiver
{
public:
    explicit RegistryObjectStackItem(QString id);

    virtual void wasAdded();

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) = 0;

protected:
    QString m_id;
};

class MonsterStackItem : public RegistryObjectStackItem
{
public:
    explicit MonsterStackItem(QString id);
    virtual void wasAdded();

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);
};

class CharacterStackItem : public StackItem
{
public:
    explicit CharacterStackItem(QString id);
};

#endif // REGISTRYOBJECTSTACKITEM_H
