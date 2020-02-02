#ifndef OBJECTREGISTRY_H
#define OBJECTREGISTRY_H

#include <QObject>
#include <QMap>
#include <QReadWriteLock>
#include <objectdata.h>
#include <gamefielddata.h>
#include "connectionhandler.h"
#include "asyncobjectreceiver.h"

class ObjectRegistry : public QObject
{
    Q_OBJECT
public:
    enum {
        TempObjectType = -1
    };
    static const QString TempObjectId;

    struct FieldDescription {
        AH::Common::GameFieldData fieldData;
        QList<AH::Common::GameFieldData::FieldOptionDescription> fieldOptions;
        QList<AH::Common::GameFieldData::FieldOptionDescription> additionalOptions;
    };

    static ObjectRegistry *instance();

    void init(ConnectionHandler *c);

    QList<FieldDescription> getFieldDescriptions() const { return m_fieldDescs.values(); }
    FieldDescription getFieldDescription(AH::Common::FieldData::FieldID id) const { return m_fieldDescs.value(id); }

    void setThisCharacterId(QString id);
    void setThisPlayerId(QString id) { m_thisPlayerId = id; }

    AH::Common::CharacterData thisCharacter() const { return m_thisCharacter; }
    QString thisCharacterId() const { return m_thisCharacterId; }
    QString thisPlayerId() const { return m_thisPlayerId; }

    bool hasObject(QString id);
    AH::Common::DescribeObjectsData getObjects(AH::Common::RequestObjectsData reqs);
    AH::Common::DescribeObjectsData::ObjectDescription getObject(QString id, AH::Common::RequestObjectsData::ObjectType type = AH::Common::RequestObjectsData::Unknown);
    template<typename T>
    T getObject(QString id);

    void asyncSubscribeObject(AsyncObjectReceiver *recv, QString id, AH::Common::RequestObjectsData::ObjectType type = AH::Common::RequestObjectsData::Unknown);
    void asyncGetObject(AsyncObjectReceiver *recv, QString id, AH::Common::RequestObjectsData::ObjectType type = AH::Common::RequestObjectsData::Unknown);
    void unsubscribe(AsyncObjectReceiver *recv);
    void unsubscribe(AsyncObjectReceiver *recv, QString id);

    AH::Common::DescribeObjectsData getObjectsOfType(QStringList ids, AH::Common::RequestObjectsData::ObjectType type);

    AH::Common::DescribeObjectsData::ObjectDescription getTempObject();
    bool hasTempObject();
    
signals:
    void objectDescribed(AH::Common::DescribeObjectsData::ObjectDescription desc);
    void thisCharacterUpdated(AH::Common::CharacterData character);
    void boardDescriptionUpdated(QList<FieldDescription> desc);

private slots:
    void receivedBoardDescription(QVariantMap board, QVariantMap desc);
    void receivedBoardUpdate(QVariantMap board);
    void receivedDescriptions(AH::Common::DescribeObjectsData descs);
    void receivedInvalidations(QStringList lst);
    void receivedTypeInvalidation(AH::Common::RequestObjectsData::ObjectType type);
    void updateCharacter(AH::Common::CharacterData character);
    void receivedMonsters(QList<AH::Common::MonsterData> monsters);
    void setTempData(const QString &data);
    void clearTempData();

private:
    ObjectRegistry();
    Q_DISABLE_COPY(ObjectRegistry)

    QReadWriteLock m_lock;
    QReadWriteLock m_subscriberLock;

    ConnectionHandler *m_conn;
    QMap<QString, AH::Common::DescribeObjectsData::ObjectDescription> m_registry;
    QMap<AH::Common::FieldData::FieldID, FieldDescription> m_fieldDescs;
    QString m_thisCharacterId;
    QString m_thisPlayerId;
    AH::Common::CharacterData m_thisCharacter;

    /*
    typedef QPair<AsyncObjectReceiver*, bool> AsyncReception;
    typedef QMap<QString, QList<AsyncReception> > AsyncReceptionMap;
    AsyncReceptionMap m_asyncRequests;
    */

    QMap<QString, QSet<AsyncObjectReceiver*> > m_subscribedMap;
    QMap<AsyncObjectReceiver*, QSet<QString> > m_subscriberMap;
    QMap<QString, QSet<AsyncObjectReceiver*> > m_singleShotSubscriberMap;

};

template<typename T>
T ObjectRegistry::getObject(QString id)
{
    if (hasObject(id)) {
        auto desc = m_registry[id];
        T t;
        desc.data >> t;
        return t;
    } else {
        return {};
    }
}

#endif // OBJECTREGISTRY_H
