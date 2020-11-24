#ifndef MYTHOSDATA_H
#define MYTHOSDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "fielddata.h"
#include "dynamicpropertydata.h"
#include "propertyvaluedata.h"

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS MythosData
        {
        public:
            MythosData();
            virtual ~MythosData() = default;

            enum MythosType {
                None,
                Headline,
                Environment,
                Rumor
            };

            enum EnvironmentType {
                Env_None,
                Env_Weather,
                Env_Urban,
                Env_Mystic
            };

            const MythosData *data() const { return this; }

            QString id() const { return m_id; }
            MythosType type() const { return m_type; }
            EnvironmentType environmenType() const { return m_envType; }
            QString name() const { return m_name; }
            QString description() const { return m_desc; }
            QString shortDescription() const { return m_shortDesc; }
            FieldData::FieldID clueField() const { return m_clueField; }
            FieldData::FieldID gateField() const { return m_gateField; }
            AH::Dimensions whiteMovement() const { return m_moveWhite; }
            AH::Dimensions blackMovement() const { return m_moveBlack; }
            virtual QList<PropertyModificationData> getModificationData() const { return m_modsData; }

            virtual QList<DynamicPropertyData> dynamicProperties() const { return m_dynProps; }

        protected:
            MythosType m_type;
            EnvironmentType m_envType;
            QString m_id;
            QString m_name;
            QString m_desc;
            QString m_shortDesc;
            FieldData::FieldID m_clueField;
            FieldData::FieldID m_gateField;
            AH::Dimensions m_moveBlack;
            AH::Dimensions m_moveWhite;
            QList<DynamicPropertyData> m_dynProps;
            QList<PropertyModificationData> m_modsData;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData::MythosType);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData::EnvironmentType);
    }
}


#endif // MYTHOSDATA_H
