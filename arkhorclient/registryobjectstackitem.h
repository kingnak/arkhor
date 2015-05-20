#ifndef REGISTRYOBJECTSTACKITEM_H
#define REGISTRYOBJECTSTACKITEM_H

#include <QObject>
#include <itemstacker.h>
#include <objectdata.h>

class RegistryObjectStackItem : public QObject, public StackItem
{
    Q_OBJECT
public:
    explicit RegistryObjectStackItem(QString id, QObject *parent = 0);

    virtual void wasAdded();

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc) = 0;

private slots:
    void objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription desc);

protected:
    QString m_id;
};

class MonsterStackItem : public RegistryObjectStackItem
{
public:
    explicit MonsterStackItem(QString id, QObject *parent = 0);
    virtual void wasAdded();

protected:
    virtual void updateObject(AH::Common::DescribeObjectsData::ObjectDescription desc);
};

#endif // REGISTRYOBJECTSTACKITEM_H
