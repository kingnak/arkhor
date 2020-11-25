#ifndef REGISTRYOBJECTSTACKITEM_H
#define REGISTRYOBJECTSTACKITEM_H

#include "itemstacker.h"
#include <objectdata.h>
#include "asyncobjectreceiver.h"

class RegistryObjectStackItem : public StackItem, public AsyncObjectReceiver
{
public:
    explicit RegistryObjectStackItem(const QString &id, bool subscribe = false);

    void wasAdded() override;
    void wasRemoved() override;

    void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) override;

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) = 0;

protected:
    QString m_id;
    bool m_subscribing;
};

class MonsterStackItem : public RegistryObjectStackItem
{
public:
    MonsterStackItem(const QString &id, double scale);
    void wasAdded() override;

protected:
    void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) override;

private:
    double m_scale;
};

class CharacterStackItem : public RegistryObjectStackItem
{
public:
    CharacterStackItem(const QString &id, double scale);
    void wasAdded() override;

protected:
    void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) override;

private:
    double m_scale;
};

#endif // REGISTRYOBJECTSTACKITEM_H
