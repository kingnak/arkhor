#include "objectregistry.h"

using namespace AH::Common;

ObjectRegistry::ObjectRegistry()
    : m_conn(NULL)
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
    connect(m_conn, SIGNAL(characterUpdate(AH::Common::CharacterData)), this, SLOT(updateCharacter(AH::Common::CharacterData)));
}

bool ObjectRegistry::hasObject(QString id)
{
    return m_registry.contains(id);
}

DescribeObjectsData ObjectRegistry::getObjects(RequestObjectsData reqs)
{
    DescribeObjectsData descs;
    RequestObjectsData pendingRequests;
    foreach (RequestObjectsData::ObjectRequest r, reqs.getRequests())
    {
        if (m_registry.contains(r.second)) {
            DescribeObjectsData::ObjectDescription d = m_registry[r.second];
            descs.addDescription(d);
        } else {
            pendingRequests.addRequest(r);
        }
    }

    if (!pendingRequests.getRequests().isEmpty()) {
        m_conn->requestObjects(pendingRequests);
    }

    return descs;
}

DescribeObjectsData::ObjectDescription ObjectRegistry::getObject(QString id, AH::Common::RequestObjectsData::ObjectType type)
{
    if (m_registry.contains(id)) {
        return m_registry[id];
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

DescribeObjectsData ObjectRegistry::getObjectsOfType(QStringList ids, RequestObjectsData::ObjectType type)
{
    RequestObjectsData reqs;
    foreach (QString id, ids) {
        RequestObjectsData::ObjectRequest r;
        r.first = type;
        r.second = id;
        reqs.addRequest(r);
    }

    return getObjects(reqs);
}

void ObjectRegistry::receivedDescriptions(DescribeObjectsData descs)
{
    foreach (DescribeObjectsData::ObjectDescription d, descs.getDescriptions()) {
        if (d.type == RequestObjectsData::Unknown) {
            qWarning("Unknown object received");
            continue;
        }
        // TODO:Verify
        /*
        QVariantMap m;
        d.second >> m;
        if (m.contains("id")) {
            m_registry[m["id"].toString()] = d;
        } else {
            qWarning("Received Object Type has no Id");
        }
        */
        m_registry[d.id] = d;
        emit objectDescribed(d);

        if (d.type == RequestObjectsData::Character) {
            CharacterData c;
            d.data >> c;
            updateCharacter(c);
        }
        /*
        switch (d.first) {

        }
        */
    }
}

void ObjectRegistry::updateCharacter(CharacterData character)
{
    if (character.id() == m_thisCharacterId) {
        m_thisCharacter = character;
        emit thisCharacterUpdated(m_thisCharacter);
    }
}
