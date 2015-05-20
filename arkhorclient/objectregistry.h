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

    static ObjectRegistry *instance();

    void init(ConnectionHandler *c);

    void setThisCharacterId(QString id) { m_thisCharacterId = id; }
    void setThisPlayerId(QString id) { m_thisPlayerId = id; }

    AH::Common::CharacterData thisCharacter() const { return m_thisCharacter; }
    QString thisCharacterId() const { return m_thisCharacterId; }
    QString thisPlayerId() const { return m_thisPlayerId; }

    bool hasObject(QString id);
    AH::Common::DescribeObjectsData getObjects(AH::Common::RequestObjectsData reqs);
    AH::Common::DescribeObjectsData::ObjectDescription getObject(QString id, AH::Common::RequestObjectsData::ObjectType type = AH::Common::RequestObjectsData::Unknown);

    AH::Common::DescribeObjectsData getObjectsOfType(QStringList ids, AH::Common::RequestObjectsData::ObjectType type);

signals:
    void objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void thisCharacterUpdated(AH::Common::CharacterData character);

private slots:
    void receivedDescriptions(AH::Common::DescribeObjectsData descs);
    void updateCharacter(AH::Common::CharacterData character);

private:
    ObjectRegistry();
    Q_DISABLE_COPY(ObjectRegistry)

    ConnectionHandler *m_conn;
    QMap<QString, AH::Common::DescribeObjectsData::ObjectDescription> m_registry;
    QString m_thisCharacterId;
    QString m_thisPlayerId;
    AH::Common::CharacterData m_thisCharacter;
};

#endif // OBJECTREGISTRY_H
