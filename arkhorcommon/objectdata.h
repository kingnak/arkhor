#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include <arkhorcommonglobal.h>
#include <serializer.hpp>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS RequestObjectsData
        {
        public:
            RequestObjectsData() {}

            enum ObjectType {
                Unknown,
                Object,
                Monster,
                Character,
                Gate,
                Mythos,
                AncientOne,
                //Board
            };

            typedef QPair<ObjectType, QString> ObjectRequest;
            typedef QList<ObjectRequest> ObjectRequestList;

            ObjectRequestList getRequests() const { return m_requests; }
            void addRequest(ObjectRequest req) { m_requests << req; }

        protected:
            ObjectRequestList m_requests;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, RequestObjectsData::ObjectRequest);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, RequestObjectsData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, RequestObjectsData::ObjectType);

        class ARKHOR_COMMON_EXPORTS DescribeObjectsData
        {
        public:
            DescribeObjectsData() {}

            //typedef QPair<RequestObjectsData::ObjectType, QVariant> ObjectDescription;
            struct ObjectDescription {
                RequestObjectsData::ObjectType type;
                QString id;
                QVariant data;
            };

            typedef QList<ObjectDescription> ObjectDescriptions;

            ObjectDescriptions getDescriptions() const { return m_descs; }
            void addDescription(ObjectDescription desc) { m_descs << desc; }

        protected:
            ObjectDescriptions m_descs;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DescribeObjectsData::ObjectDescription);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DescribeObjectsData);
        };
    }
}


#endif // OBJECTDATA_H
