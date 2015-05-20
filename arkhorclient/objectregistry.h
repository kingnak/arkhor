#ifndef OBJECTREGISTRY_H
#define OBJECTREGISTRY_H

#include <QObject>
#include <QMap>
#include <objectdata.h>
#include "connectionhandler.h"

class ObjectRegistry : public QObject
{
    Q_OBJECT
public:
    ObjectRegistry(ConnectionHandler *c);

    AH::Common::DescribeObjectsData getObjects(AH::Common::RequestObjectsData reqs);
    AH::Common::DescribeObjectsData::ObjectDescription getObject(QString id, AH::Common::RequestObjectsData::ObjectType type = AH::Common::RequestObjectsData::Unknown);

    AH::Common::DescribeObjectsData getObjectsOfType(QStringList ids, AH::Common::RequestObjectsData::ObjectType type);

signals:
    void objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription desc);

private slots:
    void receivedDescriptions(AH::Common::DescribeObjectsData descs);

private:
    ConnectionHandler *m_conn;
    QMap<QString, AH::Common::DescribeObjectsData::ObjectDescription> m_registry;
};

#endif // OBJECTREGISTRY_H
