#ifndef REGISTRYOBJECTSTACKITEM_H
#define REGISTRYOBJECTSTACKITEM_H

#include "itemstacker.h"
#include <objectdata.h>
#include "asyncobjectreceiver.h"

class RegistryObjectStackItem : public StackItem, public AsyncObjectReceiver
{
public:
    explicit RegistryObjectStackItem(QString id, bool subscribe = false);

    virtual void wasAdded();
    virtual void wasRemoved();

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc);

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) = 0;

protected:
    QString m_id;
    bool m_subscribing;
};

class MonsterStackItem : public RegistryObjectStackItem
{
public:
    MonsterStackItem(QString id, double scale);
    virtual void wasAdded();

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);

private:
    double m_scale;
};

class CharacterStackItem : public RegistryObjectStackItem
{
public:
    CharacterStackItem(QString id, double scale);
    virtual void wasAdded();

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);

private:
    double m_scale;
};

#endif // REGISTRYOBJECTSTACKITEM_H
