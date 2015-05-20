#include "objectregistry.h"

using namespace AH::Common;

ObjectRegistry::ObjectRegistry(ConnectionHandler *c)
    : m_conn(c)
{
    connect(m_conn, SIGNAL(objectDescriptions(AH::Common::DescribeObjectsData)), this, SLOT(receivedDescriptions(AH::Common::DescribeObjectsData)));
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
    d.first = RequestObjectsData::Unknown;
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
        if (d.first == RequestObjectsData::Unknown) {
            qWarning("Unknown object received");
            continue;
        }
        // TODO:Verify
        QVariantMap m;
        d.second >> m;
        if (m.contains("id")) {
            m_registry[m["id"].toString()] = d;
        } else {
            qWarning("Received Object Type has no Id");
        }
        emit objectDescribed(d);
        /*
        switch (d.first) {

        }
        */
    }
}
