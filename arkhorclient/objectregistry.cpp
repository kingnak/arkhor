#include "objectregistry.h"
#include <QEventLoop>
#include <QTimer>
#include <list>

using namespace AH::Common;

const QString ObjectRegistry::TempObjectId = "%TMP%";

ObjectRegistry::ObjectRegistry()
    : m_conn(nullptr)
{
}

ObjectRegistry *ObjectRegistry::instance()
{
    static ObjectRegistry s_inst;
    return &s_inst;
}

void ObjectRegistry::init(ConnectionHandler *c)
{
    if (m_conn) {
        qCritical("ObjectRegistry already initialized");
        return;
    }
    m_conn = c;
    connect(m_conn, SIGNAL(objectDescriptions(AH::Common::DescribeObjectsData)), this, SLOT(receivedDescriptions(AH::Common::DescribeObjectsData)));
    connect(m_conn, SIGNAL(objectInvalidations(QStringList)), this, SLOT(receivedInvalidations(QStringList)));
    connect(m_conn, SIGNAL(objectTypeInvalidation(AH::Common::RequestObjectsData::ObjectType)), this, SLOT(receivedTypeInvalidation(AH::Common::RequestObjectsData::ObjectType)));
    connect(m_conn, SIGNAL(characterUpdate(AH::Common::CharacterData)), this, SLOT(updateCharacter(AH::Common::CharacterData)));
    connect(m_conn, SIGNAL(setTempData(QString)), this, SLOT(setTempData(QString)));
    connect(m_conn, SIGNAL(clearTempData()), this, SLOT(clearTempData()));
    connect(m_conn, &ConnectionHandler::chooseMonster, this, [=](const QString&, const QList<AH::Common::MonsterData> &m) {receivedMonsters(m); });
    connect(m_conn, &ConnectionHandler::boardDescription, this, &ObjectRegistry::receivedBoardDescription);
    connect(m_conn, &ConnectionHandler::boardContent, this, &ObjectRegistry::receivedBoardUpdate);
}

void ObjectRegistry::setThisCharacterId(const QString &id)
{
    if (id != m_thisCharacterId) {
        m_thisCharacterId = id;
        m_registry.remove(id);
        //m_thisCharacter = get
        getObject(id);
    }
}

bool ObjectRegistry::hasObject(const QString &id)
{
    QReadLocker r(&m_lock);
    return m_registry.contains(id);
}

DescribeObjectsData ObjectRegistry::getObjects(const RequestObjectsData &reqs)
{
    RequestObjectsData pendingRequests;
    auto ret = doGetObjects(reqs, pendingRequests);
    if (!pendingRequests.getRequests().isEmpty()) {
        m_conn->requestObjects(pendingRequests);
    }
    return ret;
}

DescribeObjectsData ObjectRegistry::getObjectsBlocking(const RequestObjectsData &reqs)
{
    RequestObjectsData pendingRequests;
    auto ret = doGetObjects(reqs, pendingRequests);
    if (!pendingRequests.getRequests().isEmpty()) {
        QEventLoop loop;
        auto pend = pendingRequests.getRequests().toStdList();
        auto conn = connect(this, &ObjectRegistry::objectDescribed, &loop, [&](const DescribeObjectsData::ObjectDescription &desc) {
            auto it = pend.begin();
            while (it != pend.end()) {
                if (it->second == desc.id) {
                    ret.addDescription(desc);
                    it = pend.erase(it);
                } else {
                    ++it;
                }
            }
            if (pend.empty())
                loop.quit();
        });
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        m_conn->requestObjects(pendingRequests);
        loop.exec();
        ObjectRegistry::disconnect(conn);
    }

    return ret;
}

DescribeObjectsData ObjectRegistry::doGetObjects(const AH::Common::RequestObjectsData &requests, RequestObjectsData &pending)
{
    DescribeObjectsData descs;
    RequestObjectsData pendingRequests;

    {
        QReadLocker lock(&m_lock);
        for (const auto &r : requests.getRequests())
        {
            if (m_registry.contains(r.second)) {
                DescribeObjectsData::ObjectDescription d = m_registry[r.second];
                descs.addDescription(d);
            } else {
                pendingRequests.addRequest(r);
            }
        }
    }

    pending = pendingRequests;

    return descs;
}

DescribeObjectsData::ObjectDescription ObjectRegistry::getObject(const QString &id, AH::Common::RequestObjectsData::ObjectType type)
{
    {
        QReadLocker rl(&m_lock);
        if (m_registry.contains(id)) {
            return m_registry[id];
        }
    }

    if (id == TempObjectId) {
        DescribeObjectsData::ObjectDescription d;
        d.type = RequestObjectsData::Unknown;
        return d;
    }

    RequestObjectsData reqs;
    RequestObjectsData::ObjectRequest r;
    r.first = type;
    r.second = id;
    reqs.addRequest(r);

    m_conn->requestObjects(reqs);

    DescribeObjectsData::ObjectDescription d;
    d.type = RequestObjectsData::Unknown;
    return d;
}

void ObjectRegistry::asyncSubscribeObject(AsyncObjectReceiver *recv, const QString &id, RequestObjectsData::ObjectType type)
{
    {
        QWriteLocker r(&m_subscriberLock);
        m_subscribedMap[id] << recv;
        m_subscriberMap[recv] << id;
    }


    bool contains;
    AH::Common::DescribeObjectsData::ObjectDescription desc;
    {
        QReadLocker r(&m_lock);
        if ((contains =  m_registry.contains(id))) {
            desc = m_registry[id];
        }
    }

    if (contains) {
        recv->objectDescribed(desc);
    } else {
        getObject(id, type);
    }
}

void ObjectRegistry::asyncGetObject(AsyncObjectReceiver *recv, const QString &id, RequestObjectsData::ObjectType type)
{
    bool contains;
    AH::Common::DescribeObjectsData::ObjectDescription desc;
    {
        QReadLocker rl(&m_lock);
        if ((contains = m_registry.contains(id))) {
            desc = m_registry[id];
        }
    }

    if (contains) {
        recv->objectDescribed(desc);
        return;
    }

    {
        QWriteLocker rs(&m_subscriberLock);
        m_singleShotSubscriberMap[id] << recv;
    }

    getObject(id, type);
}

void ObjectRegistry::unsubscribe(AsyncObjectReceiver *recv)
{
    QWriteLocker w(&m_subscriberLock);

    // Remove from subscribers, subscribed ids
    QSet<QString> subs = m_subscriberMap.value(recv);
    m_subscriberMap.remove(recv);

    QSet<QString> toRemove;
    for (const auto &id : subs) {
        m_subscribedMap[id].remove(recv);
        if (m_subscribedMap[id].isEmpty()) toRemove << id;
    }
    for (const auto &id : toRemove) {
        m_subscribedMap.remove(id);
    }

    // Remove from single shot
    toRemove.clear();
    for (const auto &id : m_singleShotSubscriberMap.keys()) {
        m_singleShotSubscriberMap[id].remove(recv);
        if (m_singleShotSubscriberMap[id].isEmpty()) toRemove << id;
    }
    for (const auto &id : toRemove) {
        m_singleShotSubscriberMap.remove(id);
    }
}

void ObjectRegistry::unsubscribe(AsyncObjectReceiver *recv, const QString &id)
{
    QWriteLocker w(&m_subscriberLock);

    if (m_subscriberMap.contains(recv)) {
        m_subscriberMap[recv].remove(id);
        if (m_subscriberMap[recv].isEmpty()) {
            m_subscriberMap.remove(recv);
        }
    }

    if (m_subscribedMap.contains(id)) {
        m_subscribedMap[id].remove(recv);
        if (m_subscribedMap[id].isEmpty()) {
            m_subscribedMap.remove(id);
        }
    }
}

DescribeObjectsData ObjectRegistry::getObjectsOfType(const QStringList &ids, RequestObjectsData::ObjectType type)
{
    RequestObjectsData reqs;
    for (const auto &id : ids) {
        RequestObjectsData::ObjectRequest r;
        r.first = type;
        r.second = id;
        reqs.addRequest(r);
    }

    return getObjects(reqs);
}

void ObjectRegistry::setTempData(const QString &data)
{
    DescribeObjectsData::ObjectDescription obj;
    obj.data = data;
    obj.type = static_cast<RequestObjectsData::ObjectType> (TempObjectType);
    obj.id = TempObjectId;
    QWriteLocker lock(&m_lock);
    m_registry[obj.id] = obj;
}

bool ObjectRegistry::hasTempObject()
{
    QReadLocker lock(&m_lock);
    return m_registry.contains(TempObjectId);
}

void ObjectRegistry::receivedBoardDescription(QVariantMap board, QVariantMap desc)
{
    m_fieldDescs.clear();
    for (const QString &field : desc.keys()) {
        AH::Common::FieldData::FieldID fId = static_cast<AH::Common::FieldData::FieldID> (field.toInt());
        QList<AH::Common::GameFieldData::FieldOptionDescription> opts;
        desc[field] >> opts;
        GameFieldData fd;
        board[field] >> fd;
        FieldDescription d;
        d.fieldData = fd;
        d.fieldOptions = opts;
        m_fieldDescs[fId] = d;
    }
    emit boardDescriptionUpdated(m_fieldDescs.values());
}

void ObjectRegistry::receivedBoardUpdate(QVariantMap board)
{
    for (const QString &fId : board.keys()) {
        AH::Common::GameFieldData f;
        board[fId] >> f;
        if (f.id() == AH::Common::FieldData::NO_NO_FIELD) continue;
        FieldDescription fd = m_fieldDescs[f.id()];
        fd.fieldData = f;

        fd.additionalOptions.clear();
        for (const auto &o : f.fieldOptions()) {
            if (!o.id.startsWith("FIELD_")) {
                fd.additionalOptions << o;
            }
        }

        m_fieldDescs[f.id()] = fd;
    }
}

void ObjectRegistry::clearTempData()
{
    QWriteLocker w(&m_lock);
    m_registry.remove(TempObjectId);
}

void ObjectRegistry::receivedDescriptions(const DescribeObjectsData &descs)
{
    for (const auto &d : descs.getDescriptions()) {
        if (d.type == RequestObjectsData::Unknown) {
            qWarning("Unknown object received");
            continue;
        }

        {
            QWriteLocker w(&m_lock);
            m_registry[d.id] = d;
        }

        emit objectDescribed(d);

        // Update character
        if (d.type == RequestObjectsData::Character) {
            CharacterData c;
            d.data >> c;
            updateCharacter(c);
        }


        {
            // Prevent deadlock/concurrent modification when a single shot subscriber
            // is removed during notification (this is common...)
            QWriteLocker ws(&m_subscriberLock);
            QMap<QString, QSet<AsyncObjectReceiver*> > sssm = m_singleShotSubscriberMap;
            QMap<QString, QSet<AsyncObjectReceiver*> > sm = m_subscribedMap;
            sssm.detach();
            sm.detach();
            m_singleShotSubscriberMap.remove(d.id);
            ws.unlock();

            // Inform asnych subscribers and single shots
            for (auto recv : sssm.value(d.id)) {
                recv->objectDescribed(d);
            }
            for (auto recv : sm.value(d.id)) {
                recv->objectDescribed(d);
            }
        }
    }
}

void ObjectRegistry::receivedInvalidations(const QStringList &lst)
{
    QSet<QString> rerequests;

    {
        QWriteLocker l(&m_lock);
        for (const auto &id : lst) {
            m_registry.remove(id);

            if (m_subscribedMap.contains(id)) {
                rerequests << id;
            }
        }
    }

    if (!rerequests.isEmpty()) {
        AH::Common::RequestObjectsData pendingRequests;
        for (const auto &id : rerequests) {
            pendingRequests.addRequest(AH::Common::RequestObjectsData::ObjectRequest(AH::Common::RequestObjectsData::Unknown, id));
        }
        m_conn->requestObjects(pendingRequests);
    }
}

void ObjectRegistry::receivedTypeInvalidation(AH::Common::RequestObjectsData::ObjectType type)
{
    QSet<QString> rerequests;

    {
        QWriteLocker l(&m_lock);
        QList<AH::Common::DescribeObjectsData::ObjectDescription> lst = m_registry.values();
        lst.detach();
        for (const auto &obj : lst) {
            if (obj.type == type) {
                m_registry.remove(obj.id);

                if (m_subscribedMap.contains(obj.id)) {
                    rerequests << obj.id;
                }
            }
        }
    }

    if (!rerequests.isEmpty()) {
        AH::Common::RequestObjectsData pendingRequests;
        for (const auto &id : rerequests) {
            pendingRequests.addRequest(AH::Common::RequestObjectsData::ObjectRequest(AH::Common::RequestObjectsData::Unknown, id));
        }
        m_conn->requestObjects(pendingRequests);
    }
}

void ObjectRegistry::updateCharacter(const CharacterData &character)
{
    if (character.id() == m_thisCharacterId) {
        m_thisCharacter = character;
        emit thisCharacterUpdated(m_thisCharacter);
    }
}

void ObjectRegistry::receivedMonsters(const QList<MonsterData> &monsters)
{
    for (const auto &m : monsters) {
        QVariant v;
        v << m;
        AH::Common::DescribeObjectsData::ObjectDescription d{
                AH::Common::RequestObjectsData::Monster,
                m.id(),
                v
        };
        {
            QWriteLocker w(&m_lock);
            m_registry[m.id()] = d;
        }
    }
}
